SRC=src
CLASS=build/class
JAR=build/jar

SRCS=$(wildcard $(SRC)/*.java)
CLASSES=$(patsubst $(SRC)/%.java, $(CLASS)/%.class, $(SRCS))
TARGET_JAR=fuckme.jar
ENTRY_CLASS=fuckme

J_FLAGS=-sourcepath $(SRC)

.PHONY: run
.PHONY: build
.PHONY: clean

build: $(JAR)/$(TARGET_JAR)

run: $(JAR)/$(TARGET_JAR)
	java -jar $(JAR)/$(TARGET_JAR)

$(JAR)/$(TARGET_JAR): $(CLASSES)
	@mkdir -p $(@D)
	jar cfev $(JAR)/$(TARGET_JAR) $(ENTRY_CLASS) -C $(CLASS) .

$(CLASS)/%.class: $(SRC)/%.java
	@mkdir -p $(@D)
	cd $(SRC) && javac $(J_FLAGS) -d ../$(CLASS) $(subst $(SRC)/, ./, $<)

clean:
	$(RM) -r $(CLASS) $(JAR)
