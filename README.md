# QuestSystem-Plug-in-for-UE5
# Description
This project is a quest system for Unreal Engine, as well as a plugin for easy creation and editing of quests and objectives within the editor.<br />
The plugin extends the Unreal Engine editor by adding the ability to create and edit quests and their objectives in a user-friendly interface.
Users can create new quests, assign goals and update their statuses directly from the editor, as well as integrate the quest system into the project.
<br />
# Benefits: <br />
- Easy to add and manage quests through the editor. <br />
- Integration with other game systems such as UI and triggers. <br />
- Ability to subscribe to quest events, making it easy to interact with other game systems. <br />
- Ease of integration and customization in any Unreal Engine project. <br />
# Project composition
The Unreal Engine quest system plugin consists of the following main components:
1. Module (QuestEditor) <br />
This module adds a new item “Quest Editor” to the editor menu, which opens an interface for editing quests. When the menu item is activated, it opens a tab with a Quest Editor widget where you can work with quests and objectives. <br />
2. QuestEditorWidget Interface <br />
⋅⋅⋅This is an interface widget for editing quests and objectives. It provides visual tools for creating, editing and managing quests within the editor.<br />
The quest system consists of the following main components: <br />
3. QuestSystem <br />
A subsystem that manages quests in gameplay. It loads all quests from the specified Assets folder, sets the active quest, updates the statuses of objectives, and generates events when the status of a quest or objective changes. <br />
System supports events: <br />
* OnQuestActivated and OnQuestCompleted - for quests.
* OnGoalActivated and OnGoalCompleted are for the purposes of. <br /> <br /> 
Loads quests and goals from assets in the project and provides methods to change their state.  <br />
4. Data structures (QuestGoalData, QuestData) <br />
* FQuestGoalData: Description of quest objectives with unique identifier, title, description and status.
* UQuestData: Contains information about the quest, including identifier, title, description and list of objectives. <br /> <br />
The plugin is thus a complete tool for creating and managing quests in Unreal Engine, which includes an editing interface, a system for working with quest and goal data, and events for interacting with other parts of the game. <br />
# Installation and setup
1. Download the project from GitHub.
2. Copy the Plugins folder to the Plugins directory of your Unreal Engine project. <br />
![image](https://github.com/user-attachments/assets/5feae4fb-ef33-418a-b59b-0e24ac99d42b)
