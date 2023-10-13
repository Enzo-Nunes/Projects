SRC=src

SRCS=$(wildcard $(SRC)/*.java)
TARGET=xxl.app.App
TARGET_CLASS=$(SRC)/$(TARGET).class
JAVA_ARGS=-cp src/po-uilib.jar:.

.PHONY: run
.PHONY: build
.PHONY: clean

build: $(TARGET_CLASS)

run: $(TARGET_CLASS)
	cd $(SRC); java $(JAVA_ARGS) $(TARGET) 

$(TARGET_CLASS): $(SRCS)
	cd $(SRC); javac $(JAVA_ARGS) `find xxl -name "*.java"`

clean:
#	$(RM) *.class

#===OLD===
#build: $(JAR)/$(TARGET_JAR)
#
#run: $(JAR)/$(TARGET_JAR)
#	java -jar $(JAR)/$(TARGET_JAR)
#
#$(JAR)/$(TARGET_JAR): $(CLASSES)
#	@mkdir -p $(@D)
#	jar cfev $(JAR)/$(TARGET_JAR) $(ENTRY_CLASS) -C $(CLASS) .
#
#$(CLASS)/%.class: $(SRC)/%.java
#	@mkdir -p $(@D)
#	cd $(SRC) && javac $(J_FLAGS) -d ../$(CLASS) $(subst $(SRC)/, ./, $<)
#
#clean:
#	$(RM) -r $(CLASS) $(JAR)
