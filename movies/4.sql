-- determine the number of movies with an IMDb rating of 10.0.
SELECT Count(*)
FROM movies
WHERE movies.id IN
(
    SELECT ratings.movie_id
    FROM ratings
    WHERE rating = 10
    );
