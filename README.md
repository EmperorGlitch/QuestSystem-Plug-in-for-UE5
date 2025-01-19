# QuestSystem-Plug-in-for-UE5
# Idea
“External” task of the quest system plug-in for the user is the visual display: the player sees his current task and keeps track of its changes after the quest is completed. <br />
“Internal” task is to maintain a registry of all quests. In simple terms, the system has a quest system class (with attributes: id, title, list of quests), a quest class (with attributes: title, description, id). Each event has its own trigger, which allows the system to determine which quest should be currently displayed. <br />
I decided to use the HUD of the quest system from the game World of Warcraft as a basis. This decision is due to the fact that the appearance of quests in most games has similar elements, and this style turned out to be the most relevant for our project.
#
# Implementation
#
# The view on plug-in in UE5
![image](https://github.com/user-attachments/assets/b152f339-b767-4628-836a-c2101c803058)
#
