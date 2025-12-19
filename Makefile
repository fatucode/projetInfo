# Nom de l'exécutable final
EXEC = wildwater

# Compilateur et options
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

# Liste des fichiers sources (.c)
# On inclut tous les modules : Réseau, Usine, CSV, et le Main
SRCS = main.c \
       reseau.c \
       avl.c \
       avl_usine.c \
       calcul_fuites.c \
       histogramme.c \
       usine.c \
       csv_reader.c

# Transformation de la liste des .c en .o (fichiers objets)
OBJS = $(SRCS:.c=.o)

# Règle principale (par défaut)
all: $(EXEC)

# Création de l'exécutable à partir des fichiers objets
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC)

# Règle pour compiler chaque fichier .c en .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage des fichiers temporaires
clean:
	rm -f $(OBJS)

# Nettoyage complet (objets + exécutable)
fclean: clean
	rm -f $(EXEC)

# Re-compiler de zéro
re: fclean all

.PHONY: all clean fclean re
