from cs50 import get_int

# Get integer 1 to 8 inclusive
while True:
    num = get_int("Height: ")
    if num >= 1 and num <= 8:
        break

# Print #
for i in range(1, num + 1):
    print(" " * (num-i) + "#" * i + "  " + "#" * i)
