HOMEWORK 2: Ride Sharing


NAME:  Nicolas Morawski


COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

cppreference.com (regex_match)
cplusplus.com
(Used both resources for syntax references)

Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.


ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  8-10 hours


MISC. COMMENTS TO GRADER:
(optional, please be concise!)


## Reflection and Self Assessment

Discuss the issues you encountered during development and testing. What
problems did you have? What did you have to research and learn on your
own? What kinds of errors did you get? How did you fix them?

What parts of the assignment did you find challenging? Is there anything that
finally "clicked" for you in the process of working on this assignment? How well
did the development and testing process go for you?

The entire homework was relatively starightforward, all issues that occurred were
silly syntax erorrs, other than one big issue that was stopping me from compiling. With
my original plan, I was storing a default Rider object in each Driver class (to represent 
the corresponding riders' info), and vice versa. Because I was calling both the Driver and Rider
classes within each other, I created a circle dependency error that prevented my code form running.
To solve this, I simplified my code and just simply stored this info as std::strings.
