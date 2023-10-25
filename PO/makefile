SRCS=$(wildcard *.java)
TARGET=xxl.app.App
TARGET_CLASS=$(TARGET).class
JAVA_ARGS=-cp po-uilib.jar:.

.PHONY: run
.PHONY: build
.PHONY: clean

build: $(TARGET_CLASS)
build2:
	find xxl -name "*.java" -print | xargs javac -cp po-uilib.jar:.

run: $(TARGET_CLASS)
	java $(JAVA_ARGS) $(TARGET) 

$(TARGET_CLASS): $(SRCS)
	javac $(JAVA_ARGS) `find xxl -name "*.java"`

test:
	./runtests.sh

export: $(TARGET_CLASS)
	jar cvf proj.jar xxl

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
