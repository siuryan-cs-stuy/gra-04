#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"


/*======== void parse_file () ==========
  Inputs:   char * filename 
  struct matrix * transform, 
  struct matrix * pm,
  screen s
  Returns: 

  Goes through the file named filename and performs all of the actions listed in that file.
  The file follows the following format:
  Every command is a single character that takes up a line
  Any command that requires arguments must have those arguments in the second line.
  The commands are as follows:
  line: add a line to the edge matrix - 
  takes 6 arguemnts (x0, y0, z0, x1, y1, z1)
  ident: set the transform matrix to the identity matrix - 
  scale: create a scale matrix, 
  then multiply the transform matrix by the scale matrix - 
  takes 3 arguments (sx, sy, sz)
  translate: create a translation matrix, 
  then multiply the transform matrix by the translation matrix - 
  takes 3 arguments (tx, ty, tz)
  rotate: create an rotation matrix,
  then multiply the transform matrix by the rotation matrix -
  takes 2 arguments (axis, theta) axis should be x y or z
  apply: apply the current transformation matrix to the 
  edge matrix
  display: draw the lines of the edge matrix to the screen
  display the screen
  save: draw the lines of the edge matrix to the screen
  save the screen to a file -
  takes 1 argument (file name)
  quit: end parsing

  See the file script for an example of the file format


  IMPORTANT MATH NOTE:
  the trig functions int math.h use radian mesure, but us normal
  humans use degrees, so the file will contain degrees for rotations,
  be sure to conver those degrees to radians (M_PI is the constant
  for PI)
  ====================*/
void parse_file ( char * filename, 
                  struct matrix * transform, 
                  struct matrix * edges,
                  screen s) {

    FILE *f;
    char line[256];
    clear_screen(s);

    if ( strcmp(filename, "stdin") == 0 ) 
        f = stdin;
    else
        f = fopen(filename, "r");

    while ( fgets(line, 255, f) != NULL ) {
        line[strlen(line)-1]='\0';
        printf(":%s:\n",line);

        if (strcmp(line, "line") == 0) {
            double params[6];

            fgets(line, 255, f);
            line[strlen(line)-1]='\0';

            char *str = line;
            int i;
            for (i = 0; i < 6; i++) {
                params[i] = atof(strsep(&str, " "));
            }

            add_edge(edges, params[0], params[1], params[2], params[3], params[4], params[5]);
        }

        else if (strcmp(line, "ident") == 0) {
            ident(transform);
        }

        else if (strcmp(line, "scale") == 0) {
            double params[3];

            fgets(line, 255, f);
            line[strlen(line)-1]='\0';

            char *str = line;
            int i;
            for (i = 0; i < 3; i++) {
                params[i] = atof(strsep(&str, " "));
            }

            struct matrix *scale = make_scale(params[0], params[1], params[2]);
            matrix_mult(scale, transform);
            print_matrix(transform);
            free_matrix(scale);
        }

        else if (strcmp(line, "move") == 0) {
            double params[3];

            fgets(line, 255, f);
            line[strlen(line)-1]='\0';

            char *str = line;
            int i;
            for (i = 0; i < 3; i++) {
                params[i] = atof(strsep(&str, " "));
            }

            struct matrix *translate = make_translate(params[0], params[1], params[2]);

            matrix_mult(translate, transform);
            print_matrix(transform);
            free_matrix(translate);
        }

        else if (strcmp(line, "rotate") == 0) {
            fgets(line, 255, f);
            line[strlen(line)-1]='\0';

            char *str = line;
            char axis = strsep(&str, " ")[0];
            int theta = atoi(strsep(&str, " "));

            struct matrix *rot;
            if (axis == 'x') {
                rot = make_rotX(theta);
            } else if (axis == 'y') {
                rot = make_rotY(theta);
            } else {
                rot = make_rotZ(theta);
            }

            matrix_mult(rot, transform);
            print_matrix(transform);
            free_matrix(rot);
        }

        else if (strcmp(line, "apply") == 0) {
            matrix_mult(transform, edges);
        }

        else if (strcmp(line, "display") == 0) {
            clear_screen(s);

            color c;
            c.red = 0;
            c.green = 45;
            c.blue = 187;

            draw_lines(edges, s, c);
            display(s);
        }

        else if (strcmp(line, "save") == 0) {
            fgets(line, 255, f);
            line[strlen(line)-1]='\0';

            save_extension(s, line);
        }

        else if (strcmp(line, "quit") == 0) {
            break;
        }

        else {
            printf("Command not found: %s", line);
        }
    }
}
