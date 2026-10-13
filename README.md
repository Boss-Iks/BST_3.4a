# BST_3.4a
Decide which data structure will be used to store the IP information and why and implement it.

Program that:

At the close of each month, print the number of distinct IPs recorded, the average number of accesses per IP and the 5 IPs with the most accesses accumulated up to that moment.

At the end of the file, print the 15 IPs with the most accesses, each with its number of accesses, successful accesses and success percentage.

At the end of the file, allow queries by IP range: given a start and an end, show the IPs in the range with their accesses, the number of distinct IPs and the total number of accesses. Include in your submission the query for the block that goes from x.y.z.0 to x.y.z.255, where x.y.z are the first three numbers of the IP with the most accesses.

Answer questions:
Which structures from the course did you consider and discard? For each one, explain which of the monitor's conditions it does not meet.

What is the complexity of each operation of your monitor (recording a line, querying an IP, querying a range, obtaining the IPs with the most accesses)? Distinguish the best and the worst case.

What is the least favorable arrival order for your structure, and what does your evidence from step 3 show? If your structure has a weak point, how would you solve it?

Your monitor obtains the IPs with the most accesses once a month. What would you have to change if the security team wanted that information updated after every line?

From which month could the IPs that ended up with the most accesses be identified? What advantage does a live monitor have over analyzing the complete file at the end?

Look at the success percentage of your 15 IPs with the most accesses. Where does the suspicious group end and normal traffic begin? Is a high number of accesses enough to declare that an IP is malicious?

What information in each line of the log is your monitor not using? What question about the attack would you like to answer with it?
