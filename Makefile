# Nom de l'exécutable final généré
EXEC = wildwater

# Compilateur utilisé
CC = gcc

# Options de compilation :
# -Wall -Wextra : affiche les warnings
# -std=c11      : norme C utilisée
# -g            : informations de debug
CFLAGS = -Wall -Wextra -std=c11 -g

# Liste de tous les fichiers source du projet
# (réseau, AVL, usines, CSV, calculs, histogramme, main)
SRCS = main.c \
       reseau.c \
       avl.c \
       avl_usine.c \
       calcul_fuites.c \
       histogramme.c \
       usine.c \
       csv_reader.c

# Transformation automatique des .c en .o
OBJS = $(SRCS:.c=.o)

# Règle par défaut : compilation complète
all: $(EXEC)

# Création de l'exécutable final à partir des fichiers objets
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC)

# Compilation d'un fichier .c en fichier objet .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Supprime uniquement les fichiers objets
clean:
	rm -f $(OBJS)

# Supprime les fichiers objets + l'exécutable
fclean: clean
	rm -f $(EXEC)

# Recompile tout proprement
re: fclean all

# Indique que ces règles ne correspondent pas à des fichiers
.PHONY: all clean fclean re
