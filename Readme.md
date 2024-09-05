# PCom Homework #4

    In this homework, I had to implement a client that sends and receives from a server. 
I implemented some functionalities like: register, login, logout, enter_library, add_book,
delete_book, get_books, get_book. 

# requests.cpp

    In this file, I implemented the compute post and compute get requests. Both these functions were based
on the implementation from the 9th laboratory. These functions create the packages and than return them back
as strings. I also implemented the delete function that makes a similar package.

# client.cpp

    Here, I implemented the functions listed in the description. I am going to briefly describe all of them

    REGISTER: This function creates a new user and assigns a password. If there is already a user with that username,
then we cannot make the registration. 
    LOGIN: This function notifys the server that a user from its Data Base is now active, and receives a cookie 
from the server. With this cookie, the server can now gin acces to the library.
    ENTER_LIBRARY: As stated, with the cookie received from the server we can gain acces to the library. This acces is 
symbolized by an acces token. Of cours, without the first cookie, we will print at stdin that the requested operation is not permited.
    GET_BOOKS: While being logged in, and after having obtained acces to the library, a user can request acces to its books,
using the token generated previously. Now he will see all his books.
    GET_BOOK: Does the sam thing as get_books, but only shoes the requested book.
    ADD_BOOK: Adds a book to the library by sending a message to the server. After this, a get_books command will return all the 
previous books and the one that was just added.
    DELETE_BOOK : Does the exact oposite of the ADD_BOOK command.
    LOGOUT: Loggs the user out and permits to another user to be logged in.

# Usage og json.hpp
    That filse contains the a header from nlohman, that I used to parse strings, as recommended. It was very easy, beacause
the functions made the json conversion very easy.

