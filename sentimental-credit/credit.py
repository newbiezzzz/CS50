from cs50 import get_int

num = str(get_int("Number: "))

sum = 0
# start, stop, step (inclusive start, exclusive stop)
for i in range(len(num) - 1, -1, -1):
    index = len(num) - i
    # If even (starting in the end)
    if index % 2 == 0:
        n = int(num[i]) * 2
        sum = sum + n % 10 + n // 10
    # Else odd
    else:
        sum = sum + int(num[i])

# Checksum
if (sum % 10 != 0):
    print("INVALID")
# AMEX has 15 digits, starts with 34 and 37
elif (len(num) == 15 and (int(num) // 10**13 in [34, 37])):
    print("AMEX")
# VISA has 13 or 16 digits, starts with 4
elif (len(num) in [13, 16] and (int(num) // 10**12 == 4 or int(num) // 10**15 == 4)):
    print("VISA")
# MASTERCARD has 16 digits, starts with 51 to 55
elif (len(num) == 16 and int(num) // 10**14 in range(51, 56)):
    print("MASTERCARD")
else:
    print("INVALID")
