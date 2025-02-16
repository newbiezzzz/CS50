-- list the titles of all movies in which both Bradley Cooper and Jennifer Lawrence starred.


-- Find titles
SELECT title
FROM movies
-- Where movie_id is Bradley Cooper
WHERE id IN
(
    SELECT movie_id
    FROM stars
    WHERE person_id IN
    (
        SELECT id
        FROM people
        WHERE name = 'Bradley Cooper'
    )
)
-- Intersect movie_id with Jennifer Lawrence
AND id IN
(
    SELECT movie_id
    FROM stars
    WHERE person_id IN
    (
        SELECT id
        FROM people
        WHERE name = 'Jennifer Lawrence'
    )
);
