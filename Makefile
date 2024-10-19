# NAME = webserv
# CXX = c++
# CXXFLAGS = -Wall -Wextra -Werror -std=c++98
# SRCS = main.cpp Config.cpp ConfigParse.cpp ALocationheredite.cpp Server.cpp ServerConfigParse.cpp ServerConfigCommand.cpp ServerSetGet.cpp ServerException.cpp ServerStart.cpp
# OBJS_DIR = object
# DEPS_DIR = dependence
# INCLUDES = .

# # Generate object and dependency files
# OBJS = $(addprefix $(OBJS_DIR)/,$(SRCS:.cpp=.o))
# DEPS = $(addprefix $(DEPS_DIR)/,$(SRCS:.cpp=.d))

# all : ${NAME}

# $(OBJS_DIR)/%.o : %.cpp $(DEPS_DIR)/%.d
# 	@mkdir -p $(dir $@)
# 	$(CXX) $(CXXFLAGS) -c -o $@ $<

# $(DEPS_DIR)/%.d : %.cpp
# 	@mkdir -p $(dir $@)
# 	$(CXX) $(CXXFLAGS) -MM -MT $(OBJS_DIR)/$*.o -MF $@ $<

# ${NAME} : ${OBJS}
# 	${CXX} ${CXXFLAGS} ${OBJS} -o ${NAME}

# clean :
# 	rm -rf $(OBJS_DIR) $(DEPS_DIR)

# fclean : clean
# 	rm -f $(NAME)

# re : fclean all

# -include $(DEPS)

# Colors
_BLUE=\033[0;34m
_ROSE=\x1b[38;5;213m
_NEW_BLUE=\x1b[38;5;80m
_NEW_YELLOW=\x1b[38;5;228m
_GREEN_BOLD=\033[1;32m
_LIGHT_GREY=\x1b[38;5;242m
_PURPLE_UNDER=\033[0;4m\x1b[38;5;093m

# Reset colors
_END=\033[0;0m

MAKEFLAGS += --no-print-directory
OPTIONS_PRINTED = /tmp/options_printed_${USER}

SRC_DIR = srcs

SRC = \
	$(SRC_DIR)/ALocationheredite.cpp \
	$(SRC_DIR)/Config.cpp \
	$(SRC_DIR)/ConfigParse.cpp \
	$(SRC_DIR)/Server.cpp \
	$(SRC_DIR)/ServerConfigCommand.cpp \
	$(SRC_DIR)/ServerConfigParse.cpp \
	$(SRC_DIR)/ServerException.cpp \
	$(SRC_DIR)/ServerSetGet.cpp \
	$(SRC_DIR)/ServerStart.cpp \
	$(SRC_DIR)/RequestClient.cpp \
	$(SRC_DIR)/Response.cpp \
	$(SRC_DIR)/main.cpp

# $(SRC_DIR)/RequestClient.cpp \

OBJ_DIR = $(SRC_DIR)/obj
OBJ = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))

# DEPS_DIR = dependence
# DEPS = $(addprefix $(DEPS_DIR)/,$(SRC:.cpp=.d))

NAME = webserv
INC = -I./headers

RM = rm -rf
CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98

all : $(NAME)

print_options : $(OPTIONS_PRINTED)

$(OPTIONS_PRINTED) :
	@ /bin/bash -c "echo"
	@ /bin/bash -c "echo -e \"$(_ROSE)Compiling options$(_END) :  $(CC) $(CFLAGS)\""
	@ /bin/bash -c "echo"
	@ touch $(OPTIONS_PRINTED)

$(NAME) : $(OPTIONS_PRINTED) $(OBJ_DIR) $(OBJ)
	@ /bin/bash -c "echo"
	@ /bin/bash -c "echo -e \"$(_NEW_BLUE)Linking files creating binary $(NAME)$(_END)\""
	@ /bin/bash -c "echo -e \"$(CC) $(CFLAGS) $(notdir $(OBJ) $(LIBFT))\""
	@ $(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(LIB) -o $(NAME)
	@ /bin/bash -c "echo"
	@ /bin/bash -c "echo -e \"$(_GREEN_BOLD)[DONE]$(_END)\"" 

$(OBJ_DIR) :
	@ mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp 
	@ /bin/bash -c "printf \"$(_BLUE)Compiling$(_END) %-21s --> %-21s\n\" $(notdir $<) $(notdir $@)"
	@ mkdir -p $(dir $@)
	@ $(CC) $(CFLAGS) $(INC) -c $< -o $@

# $(DEPS_DIR)/%.d : %.cpp
# 	@ mkdir -p $(dir $@)
# 	@ $(CC) $(CFLAGS) $(INC) -MM -MT $(OBJ_DIR)/$*.o -MF $@ $<

clean :
	@ /bin/bash -c "echo -e \"$(_LIGHT_GREY)Cleaning object files$(_END)\""
	@ $(RM) $(OBJ_DIR)
	@ $(RM) $(OPTIONS_PRINTED)
	@ $(RM) $(DEPS_DIR)

fclean : clean
	@ /bin/bash -c "echo -e \"$(_LIGHT_GREY)Cleaning binary$(_END)\""
	@ $(RM) $(NAME)

re : fclean all

# -include $(DEPS)
