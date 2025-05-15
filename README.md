- am ales biblioteca parson pentru C recomandata in enunt
In main alocam dinamic:
- command - buffer in care citim numele comenzii
- message - buufer in care stocam cererea HTTP formata complet 
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

- login_admin -> efectueaza autentificarea pentru admin (doar admin)
* va returna cookie_login_admin, obtinand folosind strtok si strstr asupra mesajului - raspuns al serverului

- add_user -> adauga un nou utilizator normal (doar admin)
* pe langa aspectele clar precizate in enunt, verificam:
~ ca username si password sa nu contina spatii, folosind  getchar si isspace

- get_users - cere toti utilizatorii de pe server (doar admin)
* pe langa aspectele clar precizate in enunt:
~ parsam lista de utlizatori folosind functiile din biblioteca parson

- delete_user - sterge un utilizator normal (doar admin)
* pe langa aspectele clar precizate in enunt, verificam:
~ ca username sa nu contina spatii, folosind  getchar si isspace

- logout_admin - efectueaza delogarea pentru admin (doar admin)
* va returna 0, daca delogarea s-a efectuat cu succes
* -1, altfel

- login - efectueaza autentificarea pentru utilizator normal
* pe langa aspectele clar precizate in enunt, verificam:
~ ca admin_username, username si password sa nu contina spatii, folosind  getchar si isspace
* va returna cookie_login, obtinand folosind strtok si strstr asupra mesajului - raspuns al serverului

- get_access - cere acces pentru colectia de filme
* returneaza token-ul JWT

- get_movies - cere toate filmele de pe server
* pe langa aspectele clar precizate in enunt:
~ parsam lista de filme folosind functiile din biblioteca parson

- get_movie - cere informatie despre un film
* pe langa aspectele clar precizate in enunt:
~ parsam detaliile filmului (title, year, description si rating) folosind functiile din biblioteca parson

- add_movie - adauga un film
* pe langa aspectele clar precizate in enunt, verificam:
~ ca year sa fie un numar intreg pozitiv, folosind return code-ul lui scanf
~ ca rating sa fie un numar real pozitiv, cu valori intre 0 si 9.9, folosind return code-ul lui scanf

- delete_movie - sterge un film

- update_movie - actualizeaza datele unui film
* pe langa aspectele clar precizate in enunt, verificam:
~ ca id sa fie un numar intreg pozitiv, folosind return code-ul lui scanf
~ ca year sa fie un numar intreg pozitiv, folosind return code-ul lui scanf
~ ca rating sa fie un numar real pozitiv, cu valori intre 0 si 9.9, folosind return code-ul lui scanf

- get_collections - cere toate colectiile de filme de pe server
* pe langa aspectele clar precizate in enunt:
~ parsam detaliile collectiilor (id si title) folosind functiile din biblioteca parson


- get_collection - cere informatie despre o colectie de filme
* pe langa aspectele clar precizate in enunt, verificam:
~ ca id sa fie un numar intreg pozitiv, folosind return code-ul lui scanf
* apoi:
~ parsam detaliile collectiei (title, owner, lista filmelor din colectie) folosind functiile din biblioteca parson


- add_collection - adaugă o colecție de filme
- delete_collection - sterge o colectie de filme
* pe langa aspectele clar precizate in enunt, verificam:
~ ca id sa fie un numar intreg pozitiv, folosind return code-ul lui scanf


- add_movie_to_collection - adaugă un film intr-o colecție
- delete_movie_from_collection - șterge un film dintr-o colecție

- logout - efectueaza delogarea utilizatorului
* va returna 0, daca delogarea s-a efectuat cu succes
* -1, altfel

- exit - efectueaza ieșirea din program


