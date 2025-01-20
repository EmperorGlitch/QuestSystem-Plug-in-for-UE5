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
3. Copy the files from Source/QuestSystemGame/ to the Source directory/name of your game of your Unreal Engine project. <br />
![image](https://github.com/user-attachments/assets/f8d24156-0432-4bf6-9425-e2097ae4c26d)
4. Compile the project in Visual Studio (Development Editor).
5. Enable the plugin through the Unreal Engine editor:
* Go to Edit > Plugins.
* Find the Quest System plugin and activate it.
* Restart the editor.
# Examples of use
1. Open the quest editor via the Windows menu > Quest Editor. <br />
![image](https://github.com/user-attachments/assets/c76f856d-b574-4316-8dd1-b9b45e046b6c)
2. Click New Quest. <br />
![image](https://github.com/user-attachments/assets/91fd37fa-959a-47d6-a17a-80972a8997c6)
3. Enter a title, description, and add objectives (e.g., “Find an item” or “Talk to an NPC”).
4. Save Quest Save Quest.
5. Editing is done in the same interface, just make changes and click on Save Quest button as well.
6. To add a goal you need to click on the Add Goal button and enter a name and description.
7. To delete a quest, just select the quest in Quests List and click on Delete Quest button. <br /> <br />
Quests are saved in DataAssets and can be loaded into the quest system when the game level starts. 
![image](https://github.com/user-attachments/assets/dd60f336-0f19-479c-89b3-238ecc35be3b) <br />
Example of quest activation in the game: <br />
![image](https://github.com/user-attachments/assets/509d6ee7-c8db-4e30-8893-f75e506f8041) <br />
Example of updating the status of quest system goals: <br />
![image](https://github.com/user-attachments/assets/c54d90e4-e791-4bbc-a8ff-e5f61628413f) <br />
Example of subscription to completion events, quest and goal activations: <br />
![image](https://github.com/user-attachments/assets/dfbd9e00-a0dd-4425-983d-7eb04703894f) <br />
Example of using methods to visualize the current quest and goal in the UI: <br />
![image](https://github.com/user-attachments/assets/99080671-9841-4d29-a863-fb0c29217957) <br />








