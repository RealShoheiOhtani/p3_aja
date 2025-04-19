//
// Created by aidan on 4/17/2025.
//

#ifndef MOVIE_H
#define MOVIE_H

class Movie {
public:
    std::string TITLE;
    std::string ID;
    double RATING;
    int YEAR;

    Movie(std::string id, std::string title, double rating, int year) {     //constructor for student
        TITLE = title;
        ID = id;
        RATING = rating;
        YEAR = year;
    }
    std::string getTITLE() { //getter for NAME
        return TITLE;
    }
    std::string getID() {   //getter for ID
        return ID;
    }
    double getRATING() {    //getter for RATING
        return RATING;
    }
    int getYEAR() {     //getter for YEAR
        return YEAR;
    }
};


#endif //MOVIE_H
