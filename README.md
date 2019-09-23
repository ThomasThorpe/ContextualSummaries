# ContextualSummaries
An C++ program which can provide a Google-like summary of a document a list of positions of words and phrases to highlight.

# Usage
First argument is a list of word positions and spans.
Second argument is the text to provide a summary of.

# Examples
I have included multi tests files which I will describe below:
- "5toendPositions.txt" and "5toendText.txt" is an example where the 6th to last word is required to be highlighted and hence the 5 postfix wrapping words end as the text ends, this shows that "..." will not be appended no the end.
- "10spacePositions.txt" and "10spaceText.txt" is an example where there are 10 words between two words to be highlighted and hence shows that "..." will not be inserted as no words were missed.
- currentsentancePositions.txt" and "currentsentanceText.txt" shows that the context is confined to the current sentance as the specification asked for.
