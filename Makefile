
NAME = vitals-monitor
CC = gcc
C_FLAGS = -Wall -Wextra -Werror
LDF_FLAGS=-lm

GP = gnuplot
GP_FLAGS = -p -e "plot '-' with lines"

SRC_DIR = src
INC_DIR = inc
TEST_DIR = tests

RED		= "\033[0;31m"
GRN		= "\033[0;32m"  
YEL		= "\033[0;33m"
BLU		= "\033[0;34m"
BLA		= "\033[0;30m"
CYA		= "\033[0;36m"
GREY	= "\033[0;90m"
BBLA	= "\033[30;1m"
BRED 	= "\033[31;1m"
BGRN	= "\033[32;1m"
BYEL	= "\033[33;1m"
BBLU	= "\033[34;1m"
BMAG	= "\033[35;1m"
BCYA	= "\033[36;1m"
BWHI	= "\033[37;1m"
RES		= "\033[0m"

SRC = $(addprefix $(SRC_DIR)/, main.c sim_backend.c filter.c anomaly.c logger.c display.c)
TEST = $(addprefix $(TEST_DIR)/, main.c test_filter.c test_anomaly.c test_peak_detect.c test_logger.c test_hal.c)
TEST_BIN = $(TEST_DIR)/test
LOGS = log.csv

all: $(NAME)

$(NAME): $(SRC)
	@touch $(LOGS)
	@$(CC) $(C_FLAGS) -I$(INC_DIR) $(SRC) -o $@ $(LDF_FLAGS)
	@echo $(BGRN)"✨ Compilation completed ✨"$(RES)

run: $(NAME)
	./$(NAME)

plot: $(NAME)
	./$(NAME) | $(GP) $(GP_FLAGS)

test:
	@echo $(BYEL)"\nBuilding test binary...\n"$(RES)
	@$(CC) $(C_FLAGS) -I$(INC_DIR) -I$(TEST_DIR) $(TEST) $(SRC) -o $(TEST_BIN) $(LDF_FLAGS)
	@echo $(BMAG)"\n========== Test output ==========\n"$(RES)
	@./$(TEST_BIN)
	@echo $(BMAG)"\n===============================\n"$(RES)
	@echo $(BGRN)"✨ All tests passed ✨"$(RES)

clean:
	@echo $(BMAG)"✨ Nothing to clean (no object files) ✨"$(RES)

fclean:
	@rm -f $(NAME) $(TEST_BIN) $(LOGS)
	@echo $(BMAG)"✨ Program removed" $(BGRN)"successfully ✨"$(RES)

re: fclean all
	@echo $(BMAG)"✨ Re-compile was" $(BGRN)"successful ✨"$(RES)

.PHONY: all run plot test clean fclean re
