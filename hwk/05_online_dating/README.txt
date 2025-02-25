HOMEWORK 5: Online Dating


NAME:  Nicolas Morawski


COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

cppreference.com
Lectures 10 & 11 
(https://github.com/jidongxiao/CSCI1200-DataStructures/tree/master/lectures/11_list_implementation)

Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.


ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  4-6


MISC. COMMENTS TO GRADER:
(optional, please be concise!)


## Reflection and Self Assessment

Discuss the issues you encountered during development and testing. What
problems did you have? What did you have to research and learn on your
own? What kinds of errors did you get? How did you fix them?

What parts of the assignment did you find challenging? Is there anything that
finally "clicked" for you in the process of working on this assignment? How well
did the development and testing process go for you?

I did not really run into many issues, and a bit to my surprise, I almsot got all of the 
linked list and function implementation right on the first try. I ran into an issue in the
beginning with my parse_users() function, which takes in the given input file and creates
the desired linked list. However, the issue wasn't anything memeory related, but with the creation
of a user. It took me some time to realize that the premium status for each user was saved as 
'true' and 'false', rather than 'True' and 'False', so saving the values as a boolean first 
resulted in the code not running, and a debugger would not help much. Overall, the development and 
testing process was super smooth.

