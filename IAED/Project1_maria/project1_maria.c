/*
iaed-23 - ist1105875 - project1

File:  project1.c
Author:  Maria Ramos
Description: This project is a public transportation management system that
allows the definition and consultation of stations and routes.
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OUT 0
#define IN 1

/* Define the maximum number of stops, routes and connections that can be
created. */
#define MAXROUTES 200
#define MAXSTOPS 10000
#define MAXCONNECTIONS 30000

/* Define the maximum number bytes the names of the stop and routes can have. */
#define MAXNAMESTOPS 51
#define MAXNAMEROUTES 21

/* Define a struct to represent a stop, which has a name, latitude, longitude,
and the number of routes that pass through it. */
typedef struct {
    char name[MAXNAMESTOPS];
    double latitude;
    double longitude;
    int line_count;
} stop;

/* Define a struct to represent a route, which has a name, cost, duration, an
array of indices of the stops it passes through, the number of stops it contains
and an integer representing if its a cycle or not. */
typedef struct {
    char name[MAXNAMEROUTES];
    double cost;
    double duration;
    int stops_in_route;
    int is_cycle;
    int stops_index[MAXCONNECTIONS];
} route;

/* creates two arrays: one for the stops and one for the routes. */
stop stops[MAXSTOPS];
route routes[MAXROUTES];

/* creates three counters: one for the number of stops created, one for the
number of routes created, and one for the number of routes in each stop. */
int stop_count = 0;
int route_count = 0;
int line_count = 0;

/* This function prints all stops created. It loops through
the stops array and prints the name, latitude, longitude, and the number of
routes in each stop. */
void print_all_stops() {
    int i;
    for (i = 0; i < stop_count; i++) {
        printf("%s: %16.12f %16.12f %d\n", stops[i].name, stops[i].latitude,
               stops[i].longitude, stops[i].line_count);
    }
}

/* This function returns the index of the stop given if
it already exists in the stops array; otherwise, it returns -1. */
int find_stop(char stop[]) {
    int i;
    for (i = 0; i < stop_count; i++) {
        if (!strcmp(stops[i].name, stop)) {
            return i;
        }
    }
    return -1;
}

/* This function creates a new stop with the given name, latitude, and longitude
if it does not already exist in the stops array. */
void create_new_stop(char name[], double latitude, double longitude) {
    int index;
    index = find_stop(name);
    if (index != -1) {
        printf("%s: stop already exists.\n", name);
        return;
    }
    strcpy(stops[stop_count].name, name);
    stops[stop_count].latitude = latitude;
    stops[stop_count].longitude = longitude;
    stops[stop_count].line_count = 0;
    stop_count++;
}

/* This function prints the latitude and longitude of the stop given. */
void stop_info(char name[]) {
    int index;
    index = find_stop(name);
    if (index == -1) {
        printf("%s: no such stop.\n", name);
        return;
    }
    printf("%16.12f %16.12f\n", stops[index].latitude, stops[index].longitude);
}

/* This function checks the existence of quotes in a given string, returns 1 if
they exist, otherwise, returns 0. */
int check_quotes(char line[]) {
    int i;
    for (i = 0; line[i] != '\0'; i++) {
        if (line[i] == '\"') {
            return 1;
        }
    }
    return 0;
}

/* This function handles the "p" command, which creates a new stop, prints the
latitude and longitude of a given stop, or prints all stops, depending on the
command given. */
void func_p(char line[]) {
    char *token;
    char *name;
    double latitude, longitude;
    int has_quotes;

    has_quotes = check_quotes(line);

    if (has_quotes) {
        strtok(line, "\""); /* ignore everything until the name of the stop */
        token = strtok(NULL, "\""); /* get the stop's name */
        name = token;
        token = strtok(NULL, "\"");     /* get the rest of the command */
        token = strtok(token, " \n\t"); /* get latitude */

    } else {
        strtok(line, " \n\t");         /* ignore command "p" */
        token = strtok(NULL, " \n\t"); /* get next word */

        /* if theres no word after the "p", print stops, otherwise it's the
        stop's name */
        if (token == NULL) {
            print_all_stops();
            return;
        }
        name = token;
        token = strtok(NULL, " \n\t"); /* get latitude */
    }
    if (token == NULL) { /* */
        stop_info(name);
        return;
    }
    latitude = atof(token);
    token = strtok(NULL, " \n\t"); /* get longitude */
    longitude = atof(token);
    create_new_stop(name, latitude, longitude);
}

/* This function prints all routes created. It loops through the routes array
and prints the name of the route, the first and last stop, the number of stops
in the route, the cost, and the duration of the route. */
void print_all_routes() {
    int i;
    int index_first_stop, index_last_stop;

    for (i = 0; i < route_count; i++) {
        if (routes[i].stops_in_route > 0) {
            index_first_stop = routes[i].stops_index[0];
            index_last_stop =
                routes[i].stops_index[routes[i].stops_in_route - 1];
            printf("%s %s %s %d %.2f %.2f", routes[i].name,
                   stops[index_first_stop].name, stops[index_last_stop].name,
                   routes[i].stops_in_route, routes[i].cost,
                   routes[i].duration);
        } else {
            printf("%s %d %.2f %.2f", routes[i].name, routes[i].stops_in_route,
                   routes[i].cost, routes[i].duration);
        }
        printf("\n");
    }
}

/* This function returns the index of the route given if it already exists in
the routes array, otherwise, it returns -1. */
int find_route(char route[]) {
    int i;
    for (i = 0; i < route_count; i++) {
        if (!strcmp(routes[i].name, route)) {
            return i;
        }
    }
    return -1;
}

/* This function prints the stops in the route given. If the route does not
exist or has no stops, the function returns without printing anything. */
void route_info(char name[]) {
    int index_name;
    int i;
    index_name = find_route(name);

    if (routes[index_name].stops_in_route == 0) {
        return;
    }
    if (index_name == -1) {
        printf("%s: no such line.\n", name);
        return;
    }
    for (i = 0; i < routes[index_name].stops_in_route - 1; i++) {
        printf("%s, ", stops[routes[index_name].stops_index[i]].name);
    }
    printf("%s\n",
           stops[routes[index_name]
                     .stops_index[routes[index_name].stops_in_route - 1]]
               .name);
}

/* This function prints the stops in the route given in reverse order. If the
route does not exist or has no stops, the function returns without printing
anything. */
void route_info_reverse(char name[]) {
    int index_name;
    int i;
    index_name = find_route(name);

    if (routes[index_name].stops_in_route == 0) {
        return;
    }
    if (index_name == -1) {
        printf("%s: no such line.\n", name);
        return;
    }
    for (i = routes[index_name].stops_in_route - 1; i > 0; i--) {
        printf("%s, ", stops[routes[index_name].stops_index[i]].name);
    }
    printf("%s\n", stops[routes[index_name].stops_index[0]].name);
}

/* This function creates a new route with the given name if it does not already
exist in the routes array. */
void create_new_route(char name[]) {
    strcpy(routes[route_count].name, name);
    routes[route_count].cost = 0;
    routes[route_count].duration = 0;
    routes[route_count].stops_in_route = 0;
    routes[route_count].is_cycle = 0;
    route_count++;
}

/* This function handles the "c" command, which creates a new route, prints the
stops in the route given, or prints all routes created, depending on the command
given.  */
void func_c(char line[]) {
    char *token;
    char *name;

    strtok(line, " \n\t"); /* ignore the first letter */

    token = strtok(NULL, " \n\t"); /* get next word */
    if (token == NULL) {           /* if there's no word after "c" */
        print_all_routes();
        return;
    }
    name = token;

    token = strtok(NULL, " \n\t"); /* get next word if it exists */
    if (token)
        if (strcmp(token, "inverso") && strcmp(token, "invers") &&
            strcmp(token, "inver") && strcmp(token, "inve") &&
            strcmp(token, "inv")) {
            printf("incorrect sort option.\n");
            return;
        }
    if (find_route(name) == -1) {
        create_new_route(name);
        return;
    }
    if (token == NULL) {
        route_info(name);
        return;
    } else {
        route_info_reverse(name);
    }
}

/* This function returns 1 if only the first stop is in quotes, 2 if only the
second one is in quotes, and 3 if both stops are in quotes. */
int stops_in_quotes(char line[]) {
    int i;
    int first_stop;
    int last_stop;
    int space_count;
    int state = OUT;
    space_count = 0;
    first_stop = 0;
    last_stop = 0;
    for (i = 1; line[i] != '\0'; i++) {
        if (line[i] == ' ' && line[i + 1] != ' ') {
            if (state == OUT) {
                space_count += 1;
            }
        }
        if (line[i] == '\"') {
            if (state == IN) {
                state = OUT;
            } else {
                state = IN;
            }
            if (space_count == 2) {
                first_stop = 1;
            }
            if (space_count == 3) {
                last_stop = 1;
            }
        }
    }
    if (first_stop && last_stop) {
        return 3;
    }
    if (first_stop) {
        return 1;
    }
    if (last_stop) {
        return 2;
    } else {
        return 0;
    }
}

/* This function adds a connection between two stops in a given route, the stop
can be added in the beggining of the route or in the end. */
void func_l(char line[]) {

    char route_name[MAXNAMEROUTES];
    char first_stop[MAXNAMESTOPS];
    char last_stop[MAXNAMESTOPS];
    double cost;
    double duration;
    int route_index;
    int first_stop_index;
    int last_stop_index;
    int i;

    if (stops_in_quotes(line) == 0) {
        sscanf(line, "l %s %s %s %lf %lf", route_name, first_stop, last_stop,
               &cost, &duration);
    }

    if (stops_in_quotes(line) == 1) {
        sscanf(line, "l %s \"%[^\"]\" %s %lf %lf", route_name, first_stop,
               last_stop, &cost, &duration);
    }

    if (stops_in_quotes(line) == 2) {
        sscanf(line, "l %s %s \"%[^\"]\" %lf %lf", route_name, first_stop,
               last_stop, &cost, &duration);
    }

    if (stops_in_quotes(line) == 3) {
        {
            sscanf(line, "l %s \"%[^\"]\" \"%[^\"]\" %lf %lf", route_name,
                   first_stop, last_stop, &cost, &duration);
        }
    }
    first_stop_index = find_stop(first_stop);
    last_stop_index = find_stop(last_stop);
    route_index = find_route(route_name);

    /* if the route exists */
    if (route_index == -1) {
        printf("%s: no such line.\n", route_name);
        return;
    }

    /* see if stops exist */
    if (first_stop_index == -1) {
        printf("%s: no such stop.\n", first_stop);
        return;
    }
    if (last_stop_index == -1) {
        printf("%s: no such stop.\n", last_stop);
        return;
    }

    /* see if cost or duration is negative */
    if (cost < 0 || duration < 0) {
        printf("negative cost or duration.\n");
        return;
    }

    if (routes[route_index].is_cycle) {
        printf("link cannot be associated with bus line.\n");
    }

    /* if the route still doesn't have any stops */
    if (routes[route_index].stops_in_route == 0) {
        routes[route_index].stops_in_route += 2;
        routes[route_index].stops_index[0] = first_stop_index;
        routes[route_index].stops_index[1] = last_stop_index;
        routes[route_index].cost += cost;
        routes[route_index].duration += duration;
        stops[first_stop_index].line_count += 1;
        stops[last_stop_index].line_count += 1;
        return;
    }

    /* if the line is circular */
    if (last_stop_index == routes[route_index].stops_index[0] &&
        first_stop_index ==
            routes[route_index]
                .stops_index[routes[route_index].stops_in_route - 1]) {
        routes[route_index].stops_index[routes[route_index].stops_in_route] =
            find_stop(last_stop);
        routes[route_index].cost += cost;
        routes[route_index].duration += duration;
        routes[route_index].stops_in_route += 1;
        routes[route_index].is_cycle = 1;
        return;
    }

    /* if the last stop given is the first one of the route */
    if (routes[route_index].stops_index[0] == last_stop_index) {
        /* move every stop forward and add the first stop given to the
         * start of the route */
        for (i = routes[route_index].stops_in_route; i > 0; i--) {
            routes[route_index].stops_index[i] =
                routes[route_index].stops_index[i - 1];
        }
        routes[route_index].stops_index[0] = first_stop_index;
        routes[route_index].stops_in_route += 1;
        routes[route_index].cost += cost;
        routes[route_index].duration += duration;
        stops[first_stop_index].line_count += 1;
        return;
    }

    /* if the first stop given is the last one of the route */
    if (routes[route_index].stops_index[routes[route_index].stops_in_route -
                                        1] == first_stop_index) {
        routes[route_index].stops_index[routes[route_index].stops_in_route] =
            find_stop(last_stop);
        routes[route_index].cost += cost;
        routes[route_index].duration += duration;
        stops[last_stop_index].line_count += 1;
        routes[route_index].stops_in_route += 1;
        return;
    }
    printf("link cannot be associated with bus line.\n");
}

/* This function handles the "i" command, lists the stops that correspond to
route intersections. */
void func_i() {
    int i, j, k;
    int routes_with_stop_index[MAXROUTES];
    int routes_with_stop_num;
    int smallest_routes_index;
    routes_with_stop_num = 0;

    /* for every stop */
    for (i = 0; i < stop_count; i++) {
        /* for every route */
        for (j = 0; j < route_count; j++) {
            /* check all stops of the current route */
            for (k = 0; k < routes[j].stops_in_route; k++) {
                /* if the route has the stop in it */
                if (i == routes[j].stops_index[k]) {
                    /* add its' index to the list */
                    routes_with_stop_index[routes_with_stop_num] = j;
                    routes_with_stop_num += 1;
                    break;
                }
            }
        }

        /* order routes by name */
        for (j = 0; j < routes_with_stop_num; j++) {
            for (k = j; k < routes_with_stop_num; k++) {
                if (strcmp(routes[routes_with_stop_index[k]].name,
                           routes[routes_with_stop_index[j]].name) < 0) {
                    smallest_routes_index = routes_with_stop_index[k];
                    routes_with_stop_index[k] = routes_with_stop_index[j];
                    routes_with_stop_index[j] = smallest_routes_index;
                }
            }
        }

        /* if there is more than one route with the stop (a connection), print
        the name of the stop and its connections */
        if (routes_with_stop_num > 1) {
            printf("%s %d: ", stops[i].name, routes_with_stop_num);
            for (j = 0; j < routes_with_stop_num - 1; j++) {
                printf("%s ", routes[routes_with_stop_index[j]].name);
            }
            printf("%s\n", routes[routes_with_stop_index[j]].name);
        }
        routes_with_stop_index[0] = '\0';
        routes_with_stop_num = 0;
    }
}

/* This function reads commands from the input, parses them, and calls the
appropriate functions to handle each command. */
int readcommand() {
    char linha[BUFSIZ];
    fgets(linha, BUFSIZ, stdin);
    switch (linha[0]) {
    case 'q':
        return OUT;
        break;
    case 'p':
        func_p(linha);
        break;
    case 'c':
        func_c(linha);
        break;
    case 'l':
        func_l(linha);
        break;
    case 'i':
        func_i();
        break;
    }
    return IN;
}

int main() {

    while (readcommand() == IN)
        ;
    return 0;
}
