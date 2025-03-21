-- !  Creación de la base de datos mediante sentencias SQL
-- #  Fichero: creacion.sql                                                          #
-- #  Autores: Nicolas Bastida, Maximo Valenciano                                    #
-- #  Última revisión: 18/4/2024                                                     #
-- #  Resumen: Sentencias para crear la base de datos para elementos multimedia      #
-- #                                                                                 #

CREATE DATABASE multimedia;

CREATE TABLE IF NOT EXISTS Persona(
    idPersona INT PRIMARY KEY NOT NULL,
    nombrePersona VARCHAR(70) NOT NULL,
    genero VARCHAR(4)
);

CREATE TABLE IF NOT EXISTS Actor(
    idActor INT PRIMARY KEY NOT NULL, FOREIGN KEY (idActor) REFERENCES Persona(idPersona)
);

CREATE TABLE IF NOT EXISTS Realizacion(
    idRealizacion INT PRIMARY KEY NOT NULL, FOREIGN KEY (idRealizacion) REFERENCES Persona(idPersona)
);

CREATE TABLE IF NOT EXISTS Personaje(
    idPersonaje INT PRIMARY KEY NOT NULL,
    descripcion VARCHAR(100)
);

CREATE TABLE IF NOT EXISTS Obra(
    idObra INT PRIMARY KEY NOT NULL,
    titulo VARCHAR(160) NOT NULL,
    fechaProduccion INT NOT NULL
);

CREATE TABLE IF NOT EXISTS Pelicula(
    idPelicula INT PRIMARY KEY NOT NULL, FOREIGN KEY (idPelicula) REFERENCES Obra(idObra)    
);

CREATE TABLE IF NOT EXISTS Saga(
    idPelicula1 INT NOT NULL, FOREIGN KEY (idPelicula1) REFERENCES Pelicula(idPelicula),
    idPelicula2 INT NOT NULL, FOREIGN KEY (idPelicula2) REFERENCES Pelicula(idPelicula),
    nombreSaga VARCHAR(50),
    CONSTRAINT Pk_Saga PRIMARY KEY (idPelicula1, idPelicula2)
);

CREATE TABLE IF NOT EXISTS Serie(
    idSerie INT PRIMARY KEY NOT NULL,FOREIGN KEY (idSerie) REFERENCES Obra(idObra),
    fechaEmision VARCHAR(40) NOT NULL
);


CREATE TABLE IF NOT EXISTS Esta(
    idObra INT NOT NULL, FOREIGN KEY(idObra) REFERENCES Obra(idObra),
    idActor INT NOT NULL, FOREIGN KEY(idActor) REFERENCES Actor(idActor),
    CONSTRAINT Pk_Esta PRIMARY KEY(idObra, idActor)
);

CREATE TABLE IF NOT EXISTS Genero(
    idObra INT NOT NULL, FOREIGN KEY (idObra) REFERENCES Obra(idObra),
    genero VARCHAR(60) NOT NULL,
    CONSTRAINT Pk_Genero PRIMARY KEY(idObra, genero)
);

CREATE TABLE IF NOT EXISTS Trabajo(
    idRealizacion INT NOT NULL, FOREIGN KEY(idRealizacion) REFERENCES Realizacion(idRealizacion),
    trabajo VARCHAR(60) NOT NULL,
    CONSTRAINT Pk_Trabajo PRIMARY KEY(idRealizacion, trabajo)
);

CREATE TABLE IF NOT EXISTS Capitulo(
    numCap INT NOT NULL,
    numTemp INT NOT NULL,
    titulo VARCHAR(160),
    idSerie INT NOT NULL, FOREIGN KEY idSerie REFERENCES Serie(idSerie),
    CONSTRAINT Pk_Capitulo PRIMARY KEY(numCap, numTemp, idSerie)
);

CREATE TABLE IF NOT EXISTS Interpreta(
    idActor INT NOT NULL, FOREIGN KEY(idActor) REFERENCES Actor(idActor),
    idObra INT NOT NULL, FOREIGN KEY (idObra) REFERENCES Obra(idObra),
    idPersonaje INT NOT NULL, FOREIGN KEY(idPersonaje) REFERENCES Personaje(idPersonaje),
    CONSTRAINT Pk_Interpreta PRIMARY KEY(idActor, idObra, idPersonaje)
);

CREATE TABLE IF NOT EXISTS Participa(
    idRealizacion INT NOT NULL, FOREIGN KEY(idRealizacion) REFERENCES Realizacion(idRealizacion),
    idObra INT NOT NULL, FOREIGN KEY (idObra) REFERENCES Obra(idObra),
    CONSTRAINT Pk_Participa PRIMARY KEY(idRealizacion, idObra)
);