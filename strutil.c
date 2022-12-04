#include<stdio.h> 
#include<stdlib.h> 
#include<string.h>

char *substr(const char *str, size_t n) {
    char *string = strndup(str,n);
    return string;
}

char **split(const char *str, char sep) {
    int cont = 0; int i  = 0;
    while(str[i]) {
        if (str[i] == sep) {
            cont++;
        }
        i++;
    }
    cont = cont+2;
    char** string = malloc(sizeof(char*) * cont);
    unsigned int start = 0; int posicion = 0;
    for (i = 0; str[i]; i++) {
        if (str[i] == sep) {
            string[posicion] = strndup(str+start, i-start);
            posicion++;
            start = i +1;
        }
    }
    string[posicion] = strndup(str+start, i-start); posicion++;
    string[posicion] = NULL;
    return string;
}

char *join(char **strv, char sep) {
    size_t n = 0; int i = 0;
    if(!strv[i] || !strv) {
      char* string = calloc(1,sizeof(char));
      return string;
    }
    while(strv[i]) {
            n += strlen(strv[i])+1;
            i++;
    }
    char* string = malloc(sizeof(char) * n);
    char* posicion;
    i = 0; int j = 1; size_t largo = 0;
      while(strv[i]) {
          if(i == 0) {
              posicion = stpcpy(string, strv[0]);
          }
          else if (sep == '\0') {
              posicion = stpcpy(posicion, strv[i]);
          }
          else {
              posicion = stpcpy(posicion+1, strv[i]);
          }
          if (strv[j] && sep != '\0') {
              string[strlen(strv[i])+largo] = sep;
              largo += strlen(strv[i])+1;
          }
          i++;
          j++;

    }
      return string;
  }
   
   
void free_strv(char *strv[]) {
    int i = 0;
    while(strv[i]) {
        free(strv[i]);
        i++;
    }
    free(strv);
}
