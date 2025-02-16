-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Known: the theft took place on July 28, 2023
-- Known: and that it took place on Humphrey Street.

-- 1. Check crime_scene_reports
SELECT *
FROM crime_scene_reports
WHERE year = 2023 AND month = 7 AND day = 28;
-- Found witnesses of CS50 duck, id 295,
-- Took place at 10.15am at Humphrey Street Bakery.
-- 3 interviewed witnesses were present at the time

-- 2. Check interviews on
SELECT *
FROM interviews
WHERE year = 2023 AND month = 7 AND day = 28;
-- id 161, Ruth, thief get into a car an drive away, see security footage, look for cars that left the parking lot in the time frame
-- id 162, Eugene, someone recognized. ATM on Leggett Street and saw the thief there withdrawing some money
-- id 163, Raymond, thief left the backery, called someone less than a minute, planning to leave Fiftyville tomorrow in earliest flight,


-- 3. Find earliest flight id on 2023/7/29
SELECT *
FROM flights
WHERE origin_airport_id = 8
AND flights.year = 2023
AND flights.month = 7
AND flights.day = 29;
-- Return: id: 36


-- 4. Find potential thieves
-- Intersect with phone calls duration < 60 and on 2023/7/28
-- Intersect with person who flight id: 36
-- Intersect with ATM in Leggett Street on 2023/7/28
-- Intersect with car who exitted on 10.15 - 10.25 on 2023/7/28
SELECT people.name
FROM flights
JOIN passengers ON flights.id = passengers.flight_id
JOIN people ON passengers.passport_number = people.passport_number
JOIN phone_calls ON people.phone_number = phone_calls.caller
WHERE (people.phone_number IN
(
    -- Caller
    SELECT DISTINCT(caller)
    FROM phone_calls
    JOIN people ON phone_calls.caller = people.phone_number
    WHERE phone_calls.year = 2023
    AND phone_calls.month = 7
    AND phone_calls.day = 28
    AND phone_Calls.duration < 60
))
AND flights.id = 36 -- Earliest flight
-- Returned Sofia, Bruce, Kelsey, Taylor, Kenny before intersect

INTERSECT

-- Find people who withdraw in Leggett Street
SELECT people.name
FROM people
WHERE id IN
(
    SELECT person_id
    FROM bank_accounts
    WHERE account_number IN
    (
        SELECT account_number
        FROM atm_transactions
        WHERE year = 2023 AND month = 7 AND day = 28
        AND atm_location = 'Leggett Street'
        AND transaction_type = 'withdraw'
    )
)
-- Returned Kenny, Taylor, Bruce before intersect

INTERSECT

SELECT people.name
FROM people
WHERE license_plate IN
(
    -- The theft happened on 10:15
    -- The car left within 10 minutes
    SELECT license_plate
    FROM bakery_security_logs
    WHERE year = 2023 AND month = 7 AND day = 28
    AND activity = 'exit'
    AND hour = 10
    AND minute BETWEEN 15 AND 25
)
-- Returned Bruce

-- 5. Find accomplice
SELECT name
FROM people
WHERE phone_number IN
(
    SELECT receiver
    FROM phone_calls
    WHERE caller IN
    (
        SELECT phone_number
        FROM people
        WHERE name = 'Bruce'
    )
    AND year = 2023
    AND month = 7
    AND day = 28
    AND duration < 60
);
-- Returned Robin


-- 6. Check earliest flight destination id
SELECT *
FROM flights
JOIN passengers ON flights.id = passengers.flight_id
JOIN people ON passengers.passport_number = people.passport_number
JOIN phone_calls ON people.phone_number = phone_calls.caller
AND flights.id = 36;
-- Found 4

-- 7. Find airport with id 4
SELECT *
FROM airports
WHERE id = 4;
-- Return: New York City
