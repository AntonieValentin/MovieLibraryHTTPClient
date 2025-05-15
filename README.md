- am ales biblioteca parson pentru C recomandata in enunt
In main alocam dinamic:
- command - buffer in care citim numele comenzii
- message - buffer in care stocam cererea HTTP formata complet 
- response - buffer in care stocam raspunsul serverului
- cookie_login_admin - buffer in care stocam cookie-ul rezultat de comanda login_admin
* => verificam daca admin-ul este logat 
- cookie_login - buffer in care stocam cookie-ul rezultat de comanda login
* => verificam daca utilizatorul este logat
- token - buffer in care stocam token-ul JWT
* => verificam daca utilizatorul are acces la colectia de filme
* in cazul in care comanda logout_admin are succes, cookie_login_admin = NULL
* in cazul in care comanda logout are succes, token = NULL, cookie_login = NULL
Functii 
-- pentru fiecare functie, vom deschide o conexiune pe ip-ul si portul serverului (AF_INET, SOCK_STREAM)
-- mai apoi, vom compune cererea HTTP
-- o vom trimite serverului
-- in functie de mesajul - raspuns al serverului, vom afisa SUCCESS / ERROR si un mesaj suplimentar
-- la final, vom inchide conexiunea

* is_positive_integer -> returneaza 0 daca un string reprezinta un numar pozitiv, intreg, -1 altfel

1. login_admin -> efectueaza autentificarea pentru admin (doar admin)
* va returna cookie_login_admin, obtinand folosind strtok si strstr asupra mesajului - raspuns al serverului

2. add_user -> adauga un nou utilizator normal (doar admin)
* pe langa aspectele clar precizate in enunt, verificam:
~ ca username si password sa nu contina spatii, folosind  getchar si isspace

3. get_users - cere toti utilizatorii de pe server (doar admin)
* pe langa aspectele clar precizate in enunt:
~ parsam lista de utlizatori folosind functiile din biblioteca parson

4. delete_user - sterge un utilizator normal (doar admin)
* pe langa aspectele clar precizate in enunt, verificam:
~ ca username sa nu contina spatii, folosind  getchar si isspace

5. logout_admin - efectueaza delogarea pentru admin (doar admin)
* va returna 0, daca delogarea s-a efectuat cu succes
* -1, altfel

6. login - efectueaza autentificarea pentru utilizator normal
* pe langa aspectele clar precizate in enunt, verificam:
~ ca admin_username, username si password sa nu contina spatii, folosind  getchar si isspace
* va returna cookie_login, obtinand folosind strtok si strstr asupra mesajului - raspuns al serverului

7. get_access - cere acces pentru colectia de filme
* returneaza token-ul JWT

8. get_movies - cere toate filmele de pe server
* pe langa aspectele clar precizate in enunt:
~ parsam lista de filme folosind functiile din biblioteca parson

9. get_movie - cere informatie despre un film
* pe langa aspectele clar precizate in enunt:
~ parsam detaliile filmului (title, year, description si rating) folosind functiile din biblioteca parson

10. add_movie - adauga un film
* pe langa aspectele clar precizate in enunt, verificam:
~ ca year sa fie un numar intreg pozitiv, folosind return code-ul lui scanf
~ ca rating sa fie un numar real pozitiv, cu valori intre 0 si 9.9, folosind return code-ul lui scanf

11. delete_movie - sterge un film

12. update_movie - actualizeaza datele unui film
* pe langa aspectele clar precizate in enunt, verificam:
~ ca id sa fie un numar intreg pozitiv, folosind return code-ul lui scanf
~ ca year sa fie un numar intreg pozitiv, folosind return code-ul lui scanf
~ ca rating sa fie un numar real pozitiv, cu valori intre 0 si 9.9, folosind return code-ul lui scanf

13. get_collections - cere toate colectiile de filme de pe server
* pe langa aspectele clar precizate in enunt:
~ parsam detaliile collectiilor (id si title) folosind functiile din biblioteca parson


14. get_collection - cere informatie despre o colectie de filme
* pe langa aspectele clar precizate in enunt, verificam:
~ ca id sa fie un numar intreg pozitiv, folosind return code-ul lui scanf
* apoi:
~ parsam detaliile collectiei (title, owner, lista filmelor din colectie) folosind functiile din biblioteca parson

15. add_collection - adaugă o colecție de filme

16. delete_collection - sterge o colectie de filme
* pe langa aspectele clar precizate in enunt, verificam:
~ ca num_movies sa fie un numar intreg pozitiv, folosind return code-ul lui scanf
* pentru fiecare film citit, vom apela functia add_movie_to_collection_aux, care adauga fiecare film la colectie, insa fara a afisa mesaje aditionale

17. add_movie_to_collection - adauga un film intr-o colectie
* pe langa aspectele clar precizate in enunt, verificam:
~ ca collection_id si movie_id sa fie numere intregi pozitive, folosind return code-ul lui scanf

18. delete_movie_from_collection - sterge un film dintr-o colecție
* pe langa aspectele clar precizate in enunt, verificam:
~ ca collection_id si movie_id sa fie numere intregi pozitive, folosind return code-ul lui scanf

19. logout - efectueaza delogarea utilizatorului
* va returna 0, daca delogarea s-a efectuat cu succes
* -1, altfel

20. exit - efectueaza iesirea din program


