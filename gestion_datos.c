#include "dependencies/mysql.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE_VARS 200
#define INSERTS_FILE "Inserts.sql"

void init_connection(MYSQL **conn, const char *ddbb);
void close_connection(MYSQL *conn);
void send_query(MYSQL *conn, const char *query);
int count_elements(char *str, char c);
void take_out_char(char *str, char c);
int check_if_all_null(char **var, int elements);
void insert_all_from_documents(MYSQL *con, const char *dir);
void prepare_insert(char *var);

int main(void)
{
    MYSQL *conn;
    init_connection(&conn, "multimedia");
    FILE *f = fopen(INSERTS_FILE, "w");
    fprintf(f, "");
    if (f != NULL)
        fclose(f);
    insert_all_from_documents(conn, "Tablas/Persona.csv");
    insert_all_from_documents(conn, "Tablas/Obra.csv");
    insert_all_from_documents(conn, "Tablas/Actor.csv");
    insert_all_from_documents(conn, "Tablas/Realizacion.csv");
    insert_all_from_documents(conn, "Tablas/Multivaluado_trabajo.csv");
    insert_all_from_documents(conn, "Tablas/Relacion_participa.csv");
    insert_all_from_documents(conn, "Tablas/Relacion_esta.csv");
    insert_all_from_documents(conn, "Tablas/Personaje.csv");
    insert_all_from_documents(conn, "Tablas/Relacion_interpreta.csv");
    insert_all_from_documents(conn, "Tablas/Pelicula.csv");
    insert_all_from_documents(conn, "Tablas/Serie.csv");
    insert_all_from_documents(conn, "Tablas/Capitulo.csv");
    insert_all_from_documents(conn, "Tablas/Multivaluado_genero.csv");
    insert_all_from_documents(conn, "Tablas/Relacion_saga.csv");
    close_connection(conn);
    return 0;
}

void init_connection(MYSQL **conn, const char *ddbb)
{
    const char *host = "127.0.0.1";
    const char *user = "root";
    const char *password = "";
    const char *database = ddbb;
    const int port = 3306;
    if ((*conn = mysql_init(NULL)) == NULL)
    {
        printf("Error: %s", mysql_error(*conn));
        exit(1);
    }

    if (!mysql_real_connect(*conn, host, user, password, database, port, NULL, 0))
    {
        printf("%s\n", mysql_error(*conn));
        exit(1);
    }
}

void close_connection(MYSQL *conn)
{
    mysql_close(conn);
}

void send_query(MYSQL *conn, const char *query)
{
    if (mysql_query(conn, query))
    {
        printf("%s\n", mysql_error(conn));
        exit(1);
    }
}

int count_elements(char *str, char c)
{
    int count = 1;
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == c)
            count++;
    }
    return count;
}

int check_if_all_null(char **var, int elements)
{
    int aux = 0;
    for (int i = 0; i < elements; i++)
    {
        if (strcmp(var[i], ""))
            aux = 1;
    }
    return aux;
}

void take_out_char(char *str, char c)
{
    char *l = str;
    while (*l != '\0')
    {
        if (*l == c)
            *l = '\0';
        l++;
    }
}

void prepare_insert(char *var)
{
    char *aux = malloc(SIZE_VARS);
    strcpy(aux, "\"");
    strcat(aux, var);
    strcat(aux, "\"");
    strcpy(var, aux);
    if (aux != NULL)
    {
        free(aux);
        aux = NULL;
    }
}

void insert_all_from_documents(MYSQL *conn, const char *dir)
{
    FILE *lectura = fopen(dir, "r");
    char insert[300] = "INSERT INTO ";
    char insert_copy[300];
    int elements = 0;
    char linea[300];
    int aux = 0;
    char *table = malloc(40);
    fgets(table, 40, lectura);
    take_out_char(table, '\n');
    strcat(insert, table);
    strcat(insert, " (");
    FILE *escritura = fopen(INSERTS_FILE, "a");
    fgets(linea, 300, lectura);
    elements = count_elements(linea, ';');
    take_out_char(linea, '\n');
    char *data = strtok(linea, ";");
    while (aux < elements)
    {
        strcat(insert, data);
        if (aux != elements - 1)
            strcat(insert, ", ");
        data = strtok(NULL, ";");
        aux++;
    }
    char *var[elements];
    for (int i = 0; i < elements; i++)
        var[i] = malloc(SIZE_VARS);
    strcat(insert, ") VALUES (");
    strcpy(insert_copy, insert);
    if (lectura == NULL)
        return;
    while (!feof(lectura))
    {
        strcpy(insert, insert_copy);
        for (int i = 0; i < elements; i++)
        {
            var[i][0] = '\0';
            if (elements == 1)
                fscanf(lectura, "\r\n%[^\n]s", var[i]);
            else if (i == 0)
                fscanf(lectura, "\r\n%[^;]s;", var[i]);
            else if (i == elements - 1)
                fscanf(lectura, ";%[^\n]s\r\n", var[i]);
            else
                fscanf(lectura, ";%[^;]s;", var[i]);
        }

        if (check_if_all_null(var, elements))
        {
            for (int i = 0; i < elements; i++)
            {
                if (strcmp(var[i], ""))
                    prepare_insert(var[i]);
                strcat(insert, (!strcmp(var[i], "")) ? "NULL" : var[i]);
                if (i != elements - 1)
                    strcat(insert, ", ");
            }
            strcat(insert, ");");
            fprintf(escritura, "%s\n", insert);
            send_query(conn, insert);
        }
    }
    for (int i = 0; i < elements; i++)
    {
        if (var[i] != NULL)
        {
            free(var[i]);
            var[i] = NULL;
        }
    }
    if (data != NULL)
    {
        free(data);
        data = NULL;
    }
    if (table != NULL)
    {
        free(table);
        table = NULL;
    }
    if (lectura != NULL)
        fclose(lectura);
    if (escritura != NULL)
        fclose(escritura);
}