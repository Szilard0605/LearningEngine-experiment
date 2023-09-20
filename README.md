<img align="left" src="Content/Branding/LOGO.jpg" width="100px"/>

# LearningEngine

This is the more advanced version of my Engine which is being built for the reason for me to understand how Game Engines work and how to make them.
In this version i am planning to make it more like a Game Engine, i am going to add an Editor, make it perform faster and i am going to focus on 
3D. My plan is to make a Multiplayer game with this Engine. It will be a _long_, but **fun** journey.

# Development Setup

LearningEngine is currently only set up for development on the *Windows* operating system in Visual Studio.

Clone the LearningEngine repository to your machine and run `scripts/Win-GenProjects.bat`. 
This will generate solution files that you can open in Visual Studio. You'll then find LearningEngine.sln in the project root.

After opening the solution, Sandbox should be the active project by default, and pressing the play button in Visual Studio should open up a demo scene.

You may find that Sandbox crashes because it cannot find the assets it needs to run. One way to resolve this is to right click on 
the `Sandbox` project, open `Properties`, go to `Configuration Properties`, `Debugging`, and change `Working Directory` to `$(SolutionDir)/Sandbox`. 
Do the same thing with `LE_Editor`, except target the `LE_Editor` folder as the `Working Directory`.

# Development Workflow

1. When you begin work on a new task, start by creating a new branch off of _master_, and give it a name that is relevant to the specific task you are
working on.

2. Pull master into your development branch regularly. If someone merges a change to a file you are actively working on, you will want to resolve any
conflicts as soon as you can.

3. Once you finish the task, create a merge request to merge your branch into master. In general, let at least one other developer review your code
before it is merged into master. Also, in general, do not merge any breaking changes into master (crashes, broken features, etc.) that will slow or stop
developers working on other tasks.

By adhering to these 3 guidelines, you avoid having to contend with any broken or in-progress changes on other development branches for unrelated tasks.

## Images of the development
#### Images of the Editors early version with a 3D render made with it:
<img align="left" src="Content/Showcasing/editor_sr.JPG"/> 
<p align="center">2023.05.25</p>

<img align="left" src="Content/Showcasing/2023.05.25.JPG"/>  
<p align="center">2023.05.25</p>


