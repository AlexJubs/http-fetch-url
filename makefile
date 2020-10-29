RM = rm -rf

all: url_cli

url_cli: url_cli.c
	gcc $< -lcurl -o url_cli

clean:
	$(RM) url_cli

.PHONY: all

