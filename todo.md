# Text Widgets
- Change font color
- Change font alignment

# Numbers
- Separate integer and double widgets
- Some form of graph
- Slider bar
- Improve the dial; 360-degree range

# Enum Widget
- Can display certain colors depending on the string, double, or int value of a topic.
- Should be infinitely configurable; at least 50 value-color combos.

# Bool Widgets
- Edit checkbox size
- Edit true/false colors

# Field2d
- Exactly as in Shuffleboard.

# FMSInfo
- yes

# Commands
- Yes

# Theming
- More themes
- Background color for titles

# Stuff I'll do way after I leave FRC
- Drag & Drop

# Custom Widgets; rando mthoughts I had
- every custom widget must still be a class (use QPluginLoader)
- HOWEVER, They just need to be created and have a few metaproperties defined.
    * DisplayType (QMetaType, any supported type)
- Define and lay out a few widgets
- Define metaproperties for each necessary property (e.g. min value)
- Bind class metaproperties to widget metaproperties (e.g. QAbstractSlider::minimum)

- BaseWidget can generate a new/resize dialog based upon these metaproperties
- Widgets are layed out according to the definition in the plugin class.
