TEX = pdflatex
CP = cp
RM = rm -f
OUT = results.pdf
PDF_DIR = ..
SRCS = results.tex abstract.tex introduction.tex hdr.tex openexr.tex tonemapping.tex documentation.tex bibliography.tex

all: $(OUT)

$(OUT): $(SRCS)
	$(TEX) results.tex
	$(CP) $(OUT) $(PDF_DIR)/$(OUT)

clean:
	$(RM) $(PDF_DIR)/$(OUT) $(OUT)
