
TARGETS = SHELL

all: $(TARGETS)

SHELL:
	@chmod +x ./*.sh
	@./conv.sh all
	@make -C ConndServerSrc 
	@make -C ClientSrc

clean:
	@make -C ConndServerSrc clean
	@make -C ClientSrc clean
