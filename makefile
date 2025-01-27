# Vars
NAME = libmemlib.a
CC = gcc
# CFLAGS = -Wall -Wextra -Werror
AR = ar
ARFLAGS = rcs
SRCS = src/heap.c src/fn_memcpy.c src/fn_memset.c src/fn_align_size.c
OBJ = $(SRCS:.c=.o)
BONUS_OBJS = $(BONUS_SRCS:.c=.o)

# Rules
all: $(NAME)

$(NAME): $(OBJ)
	@$(AR) $(ARFLAGS) $(NAME) $(OBJ)
	@echo "Library $(NAME) created."

%.o: %.c include/heap.h
	@$(CC) $(CFLAGS) -c $< -o $@

bonus: $(BONUS_OBJS)
	@$(AR) $(ARFLAGS) $(NAME) $(BONUS_OBJS)
	@echo "Bonus objects added to $(NAME)."

clean:
	@rm -f $(OBJ)
	@rm -f $(BONUS_OBJS)
	@echo "Object files cleaned."

fclean: clean
	@rm -f $(NAME)
	@echo "Library $(NAME) removed."

re: fclean all
