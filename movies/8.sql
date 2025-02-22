--  list the names of all people who starred in Toy Story.
SELECT name
FROM people
WHERE people.id IN
(
    SELECT person_id
    FROM stars
    WHERE movie_id IN
    (
        SELECT id
        FROM movies
        WHERE title = 'Toy Story'
    )
)
