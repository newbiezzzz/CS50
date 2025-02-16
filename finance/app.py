import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    '''Need Stocks symbol, Stock shares, Current stock price, Current cash, Grand total'''
    # Get id from session
    user_id = session["user_id"]

    # Get the current cash
    cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]
    cash = round(cash, 2)

    # Get stock symbol, stock shares
    stocks = db.execute("SELECT * FROM stocks WHERE user_id = ?", user_id)

    stock_total = 0
    # Append current stock price to buys
    for stock in stocks:
        stock["price"] = lookup(stock["symbol"])["price"]
        stock["total"] = round(stock["price"] * stock["shares"], 2)  # Round to 2 decimal places
        stock_total += stock["total"]

    # Stocks + cash
    total = round(stock_total + cash, 2)

    return render_template("index.html", cash=cash, stocks=stocks, total=total, usd=usd)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("must provide symbol", 400)

        symbol = lookup(request.form.get("symbol"))
        if not symbol:
            return apology("symbol do not exist", 400)

        if not request.form.get("shares"):
            return apology("must provide shares", 400)

        # Avoid client-side non integer inputs
        try:
            shares = int(request.form.get("shares"))
        except ValueError:
            return apology("shares must be integer", 400)

        # print(request.form.get("shares")) # Returns string
        # Avoid negative integers
        if int(request.form.get("shares")) <= 0:
            return apology("shares must be positive integer", 400)

        # Get id from session
        user_id = session["user_id"]

        # Query database for cash
        cash = db.execute(
            "SELECT cash FROM users WHERE id = ?", user_id
        )[0]["cash"]

        # Look up stock price after checking
        symbol = lookup(request.form.get("symbol"))

        # Calculate total price
        total_price = symbol["price"] * shares

        # If not enough cash, apologize
        if cash < total_price:
            return apology("Not enough cash", 403)

        # Insert to stocks table
        db.execute(
            "INSERT INTO stocks (user_id, symbol, shares) values (?, ?, ?)"
            "ON CONFLICT(user_id, symbol) DO UPDATE SET shares = shares + ?",
            user_id, symbol["symbol"], shares,
            shares
        )

        # Insert to transactions table
        db.execute(
            "INSERT INTO transactions (user_id, symbol, shares, price, status) values (?, ?, ?, ?, ?)",
            user_id, symbol["symbol"], shares, symbol["price"], "buy"
        )

        # Extract money from user_id and update database
        cash = cash - total_price
        db.execute(
            "UPDATE users SET cash = ? WHERE id = ?", cash, user_id
        )

        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session["user_id"]
    trans = db.execute("SELECT * FROM transactions WHERE user_id = ?", user_id)
    return render_template("history.html", trans=trans, usd=usd)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("must provide symbol", 400)

        symbol = lookup(request.form.get("symbol"))
        if not symbol:
            return apology("symbol do not exist", 400)
        return render_template("quoted.html", symbol=symbol, usd=usd)

    # Method "GET"
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Ensure confirmation was submitted
        elif not request.form.get("confirmation"):
            return apology("must provide confirmation", 400)

        # If password and confirmation do not match
        if (request.form.get("password") != request.form.get("confirmation")):
            return apology("password and confirmation do not match", 400)

        # Insert username to database
        try:
            db.execute(
                "INSERT INTO users (username, hash) values (?, ?)", request.form.get(
                    "username"), generate_password_hash(request.form.get("password"))
            )
        # If username already exist, apologize.
        except ValueError:
            return apology("Username already exists", 400)

        # Done with register, render login page
        return render_template("login.html")

    # Render register page if "GET"
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    user_id = session["user_id"]
    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("must provide symbol", 400)

        symbol = lookup(request.form.get("symbol"))
        if not symbol:
            return apology("symbol do not exist", 400)

        if not request.form.get("shares"):
            return apology("must provide shares", 400)

        # Avoid client-side non integer inputs
        try:
            shares = int(request.form.get("shares"))
        except ValueError:
            return apology("shares must be integer", 400)

        # Avoid negative integers
        if int(request.form.get("shares")) <= 0:
            return apology("shares must be positive integer", 400)

        # Check if the user has that many stock or have the specific stock??
        stock = db.execute(
            "SELECT symbol, shares FROM stocks WHERE user_id = ? AND symbol = ?", user_id, symbol["symbol"])

        # If do not have stock
        if not stock:
            return apology("do not have the stock", 400)

        # If not enough stock render apology
        if stock[0]["shares"] < shares:
            return apology("not enough shares", 400)

        # If all checks success then sell
        sell_price = symbol["price"] * shares

        # Query database for cash
        cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]

        # Sum cash with sold stock price
        cash += sell_price

        # Leftover stock's shares
        stock[0]["shares"] -= shares

        # Insert to transactions table
        db.execute(
            "INSERT INTO transactions (user_id, symbol, shares, price, status) values (?, ?, ?, ?, ?)",
            user_id, symbol["symbol"], shares, symbol["price"], "sell"
        )

        # Update stock if stock sold
        if stock[0]["shares"] != 0:
            db.execute(
                "UPDATE stocks SET shares = ? WHERE user_id = ? AND symbol = ?",
                stock[0]["shares"], user_id, stock[0]["symbol"]
            )
        # Delete the stock since no more shares
        else:
            db.execute("DELETE FROM stocks WHERE user_id = ? AND symbol = ?",
                       user_id, stock[0]["symbol"])

        # Update money if stock sold
        db.execute(
            "UPDATE users SET cash = ? WHERE id = ?", cash, user_id
        )

        # Render index.html if sell successfully
        return redirect("/")

    else:
        stocks = db.execute("SELECT symbol FROM stocks WHERE user_id = ?", user_id)
        return render_template("sell.html", stocks=stocks)


@app.route("/add", methods=["GET", "POST"])
@login_required
def add():
    ''' Add money '''
    user_id = session["user_id"]
    if request.method == "POST":
        if not request.form.get("cash"):
            return apology("must enter amount", 400)

        # Avoid client-side non integer inputs
        try:
            cash = int(request.form.get("cash"))
        except ValueError:
            return apology("amount must be integer", 400)

        # Avoid negative integers and zero
        if int(request.form.get("cash")) <= 0:
            return apology("amount must be positive integer", 400)

        cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]
        print(cash)
        cash += int(request.form.get("cash"))
        print(cash)

        # Update cash in users table
        db.execute("UPDATE users SET cash = ? WHERE id = ?", cash, user_id)
        return redirect("/")
    else:
        return render_template("add.html")
