# journey-code
Making a jouney app to practice external files

* l32/l39 - dayformarter() was implemented to solve problem to DDMMYY formart.
* l41/l74 - newJourney() <- dayformarter()
* 141/174 newJourney() algorithm for calculating missing days was solved finaly!
* Minor improvements in newJournay function.
* All bug has fixed with Texteditor.h, colors.h.

# TO-DO

+ Texteditor.h implemented to handle with journeys edition. see programadory/text-editor.git [DONE]
+ Create the path for pages and put all pages there, When a user intend to create a new page but it's the same day compare the currente data with the last item on vector. [DONE]
+ Create features like, Edit journal, Delete etc...

# BUGS REPORTED

- it will be nedded set a limit to create newJourneys, limitating 1 by day! [FIXED]
- Memory leaks within stdout, it's caching the buffer. (colors.h scape bug probably); [FIXED]
- BUG segmentation fault with myLog struct, caused when calling to much pushs inside newJourney function. undefined stack<> size behaviour [FIXED]
- 

# EXTRAS!

+ LOG implemeted in Termux.h to help with debug [DONE]
+ Small improvements for LOG in Termux.h and bug fixes..