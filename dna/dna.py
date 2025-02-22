import csv
import sys


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        print("python dna.py example.csv example.txt")
        sys.exit(0)

    # TODO: Read database file into a variable
    datas = []
    with open(sys.argv[1]) as file:
        reader = csv.DictReader(file)
        for data in reader:
            datas.append(data)

    sequence = []
    # TODO: Read DNA sequence file into a variable
    with open(sys.argv[2]) as file:
        reader = file.read()
        for seq in reader:
            sequence.append(seq)

    # TODO: Find longest match of each STR in DNA sequence
    strs = list(datas[0].keys())[1:]
    strs_seperated = []
    # Seperate strs words to letters
    for i in range(len(strs)):
        store = []
        for j in range(len(strs[i])):
            store.append(strs[i][j])
        strs_seperated.append(store)

    # Get longest match of all subsequence given sequence
    longest = []
    for str_s in strs_seperated:
        longest.append(longest_match(sequence, str_s))

    # TODO: Check database for matching profiles
    for i in range(len(datas)):
        data = list(datas[i].values())[1:]

        # Convert str to int
        for j in range(len(data)):
            data[j] = int(data[j])

        # If match print name
        if data == longest:
            print(list(datas[i].values())[0])
        else:
            continue
    else:
        print("No match")

    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
