import cs50


def main():
    # Get string
    text = cs50.get_string("Text: ")

    # Count text
    letters = count_letters(text)
    words = count_words(text)
    sentences = count_sentences(text)

    # Calculate index
    L = letters / words * 100
    S = sentences / words * 100
    index = 0.0588 * L - 0.296 * S - 15.8
    index_rounded = round(index)

    # Print grade
    if (index < 1):
        print("Before Grade 1")
    elif (index >= 16):
        print("Grade 16+")
    else:
        print(f"Grade {index_rounded}")


def count_letters(text):
    count = 0
    for i in range(len(text)):
        # If character is alphabet, increment count
        if text[i].isalpha():
            count = count + 1
    return count


def count_words(text):
    count = 0
    for i in range(len(text) - 1):
        curr = text[i]
        next = text[i + 1]
        # If current is not space and next is space, increment count
        if curr != ' ' and next == ' ':
            count = count + 1
    if next != ' ':
        count = count + 1
    return count


def count_sentences(text):
    count = 0
    for i in range(len(text) - 1):
        curr = text[i]
        next = text[i + 1]
        # If current is alphabet and next is ['.', '?', '!'], increment count
        if curr.isalpha() == True and (next in ['.', '?', '!']):
            count = count + 1
    return count


main()

