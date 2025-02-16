-- names of all people who starred in a movie released in 2004, ordered by birth year.

-- find name in people
SELECT name
FROM people
WHERE id IN
(
    -- Find movie_id with stars table
    SELECT person_id
    FROM stars
    WHERE movie_id IN
    (
        -- Find movie released in 2004
        SELECT id
        FROM movies
        WHERE year = 2004
    )
)
ORDER BY birth;
