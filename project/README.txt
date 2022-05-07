ICP Project 2022
UML Editor for Class and Sequence Diagrams

Authors: 
Martin Talajka (xtalaj00)
Ondrej Kováč (xkovac57)

Installation:
-use 'make' in the main folder to build the project
-use 'make run' to build the project and run it 
-use 'make doxygen' to generate an html documentation for the project
-use 'make clean' to clean the directory after build
-use 'make pack' to create a zip of the project

How to use:

Main window:
-Use 'New Project' (Ctrl + N) to create a new project
-Use 'Open Project' (Ctrl + O) to open an existing project from a .json file
-Use 'Save As...' to save the file to a specified location with a specified name
-Use 'Save' (Ctrl + S) to save the project
-Use 'Undo' (Ctrl + Z) to undo the last action (multiple steps back)
-Use 'Redo' (Ctrl + Y) to redo the last undone action
-Use 'Documentation' (F1) to show documentation page (if it was generated)
-Use 'Exit' (Ctrl + Q) to close the project window.
-Use the tabs 'Class Diagram' and 'Sequence Diagram' to switch between the 2 modes

Class Diagram:

Classes:
-To create a new Class, click with the right mouse button into a blank space in the scene and select 'New Class'
-In the Class Element you can change the name of the class, add Attributes and Methods or delete the Element by clicking the "R" button
-You can change the names of Attributes and Methods, you can select from 4 Visibility types (public, private, protected and package) and you
    can delete the Attributes and Methods by clicking the "R" button next to them

Connections:
-To create a connection between the classes, right click one class and then right click the second one
-To assign arrows to the connections between the classes, select the connection by clicking on it and right-click on a blank space in the scene
    From there you can pick the arrow for the first connection (source of the line), second connection (destination of the line) or remove the connection
-You can choose one of the four types of arrows: Association, Generalization, Composition, Aggregation

Sequence Diagram:

Objects:
-To create a new Object, click with the right mouse button into a blank space in the scene and select 'New Object'
-A name can then be assigned to the object and a class can be picked from the pool of already created classes in the Class Diagram
-The pool of available classes automatically updates when the class diagram is changed
-To create a life line of the object, click on the "+" button of the object. This will create a life line and spawn the first Message Anchors
-Pressing the "+" button further will make the life line longer and create additional Message Anchors
-The object also features 2 "side anchors" used to simulate object creation
-Pressing the "-" button will shorten the life line and remove Message Anchors one by one. This only works up till the last anchor
-When an anchor is deleted all the messages, activations and destruction icons are removed with it
-Pressing the "Del" button will delete the object

Actors:
-Actors behave very similiarly to objects
-Actors however do not belong to a class, do not have side anchors and cannot receive named messages

Messages:
-To create a message, left click one of the anchors and left click on another one
-If the target is an object a Combo Box will appear as well
-If the target object belongs to a class, the method of the message can be picked from the available pool of methods
-The pool of methods is automatically updated with changes to the class diagram
-Generalization of classes also translates to the sequence diagram (sort of, read known bugs section)
-If the target is an actor, no Combo Box appears
-To change the type of the message (Synchronous, Asynchronous and Return) or remove it, select it by left clicking and then open a context menu by
    right-clicking on a blank space in the scene

Activations:
-To simulate a working object or actor an Activation can be spawned
-Spawn the activation by right clicking a source Message Anchor and then right clicking a target Message Anchor on the same life line

Destruction Icons:
-To simulate a destruction of an object, double-click an anchor to mark it as a destruction point
-Double-click the marked anchor to remove the destruction icon

Missing Implementation:
-Multiple sequence diagrams -> theoretically, different sequence diagrams can be modeled into the same scene
-The Visibility setting for methods and attributes is purely cosmetic and does not affect the sequence diagram
-User-friendly color coding and differentiation of selected objects
-Proper inconsistency handling when loading a "corrupted" (wrongly edited) .json file
-Add custom text button visible in the condext menu for both scenes


Note on crashes: 
There was not enough time to properly debug the program and crashes are likely in order.
The ones we found were fixed but the program can still likely crash.

Known bugs:
1. Messages available through generalization in sequence diagram will lose their name and have to be re-clicked on load (the arrow itself does not disappear)
2. Generalization arrows and messages work properly only if the generalization arrow in class diagram is set as the "Second connection"
   (if set as first connection generalization will not work porperly in sequence diagram)
3. Arrows in class diagram may appear bugged when created -> move the class element around to fix it
4. If an element is selected in class diagram and the user switches to sequence diagram, upon opening the context menu with right click, the
   menu will still reference the element context menu from previous diagram and vice versa -> fix by clicking on an empty space in the diagram with left click before switching diagrams
5. Activation cannot be deleted from sequence diagram -> delete it by deleting the anchors until the activation is reached
6. The Combo Box next to a message may still show an out of-date method after the class of the targeted object has been removed from the class diagram


Design patterns (sourced from https://www.oodesign.com):
Singleton - Ensure that only one instance of a class is created and Provide a global access point to the object
    -Example: ProjectManager Class -> only one projectManager instance exists in the whole program and can be accessed via a pointer in WorkScene
Flyweight - use sharing to support a large number of objects that have part of their internal state in common where the other part of state can vary
    -Example: Multiple objects are created and saved in vectors with their own attributes
Observer - Define a one-to-many dependency between objects so that when one object changes state, all its dependents are notified and updated automatically
    -Examples: Class pools are edited for all objects if a class is created or deleted, same goes for methods and messages