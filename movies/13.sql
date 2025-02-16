-- list the names of all people who starred in a movie in which Kevin Bacon also starred.

SELECT DISTINCT name
FROM people
-- JOIN people -> stars -> movies
-- Combine table to find all people who starred in the all the movies
JOIN stars ON people.id = stars.person_id
JOIN movies ON stars.movie_id = movies.id
-- From that table, find the movie.id that Kevin Bacon starred
WHERE movies.id IN
(
    SELECT movies.id
    FROM movies
    JOIN stars ON movies.id = stars.movie_id
    JOIN people ON stars.person_id = people.id
    WHERE people.name = 'Kevin Bacon'
)
-- Exclude Kevin Bacon
AND people.name != 'Kevin Bacon';
