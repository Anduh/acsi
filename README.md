# ACSI - Automated Character Sheet Internationalizer
![version](https://img.shields.io/badge/version-0.5-yellowgreen.svg) ![platform](https://img.shields.io/badge/platform-Linux-lightgrey.svg) ![stars](https://img.shields.io/github/stars/Anduh/acsi.svg?style=social)

![sheets translated](https://img.shields.io/badge/Sheets%20Translated-5-blue.svg) ![GitHub last commit](https://img.shields.io/github/last-commit/Anduh/acsi.svg)

# Basic Overview

**ACSI** is a simple program to partially automate the [internationalization](https://wiki.roll20.net/Character_Sheet_i18n) of existing [Character Sheets](https://github.com/Roll20/roll20-character-sheets) made for the [Roll20 platform](https://en.wikipedia.org/wiki/Roll20). It takes a html file, creates a copy where it places `i18n`-tags according to some of the [Character Sheet i18n](https://wiki.roll20.net/Character_Sheet_i18n) standard. Works on Linux.

# Table of contents

* [Install/Run](#installrun)
	* [Tested platforms](#tested-platforms)
* [Use](#use)
	* [Constraints](#constraints)
    * [Options](#otions)
* [Changelog](#changelog)
* [Project Plan](#project-aim-and-development-roadmap)
* [Glossary](#glossary)

# Install/Run
- **Option 1:** Download and install the latest [.deb](https://github.com/Anduh/acsi/raw/master/linux/) package from the `/linux/` folder.
- **Option 2:** Download the project(If you have `git` installed you can just run the command `git clone https://github.com/Anduh/acsi.git` and  it downloads everything)
    - Then open the command line in the `acsi` folder and run `make` command. ACSI should compile now from the files in the `/src` folder into a new program file simply named `acsi`. (This is not a complete install, only create a local program that can be run inside the folder)


If it throws errors such as `missing file stdio.h` or similar, you might need to also install the  `build-essential` package.

## Tested platforms
List of operating systems that ACSI have been successfully installed/compiled on.

**Linux:**
- Ubuntu 16.4 TLS
- Ubuntu 18.4 TLS
- Linux Mint 19.1 (required install of `build-essential` package)

# Use
If you installed ACSI with **Optinon 1**, you can now call ACSI from the command line with: `acsi <filename>`, where you replace `<filename>` with the name of your file you want translated(must be a `.html` file). Your command line terminal need to be open in the location where your file is located.

If you only compiled ACSI with **Option 2**, you must place the html file you want translated in the same folder as ACSI is located. You can now call ACSI from the command line with: `./acsi <filename>`.

**NOTE:** In V0.5, `rolltemplates` and `sheetworkers` should be removed from the HMTL so ACSI won't trip and make changes to them.

ACSI will read the html file it is given and create a html file(named `<originalname>-i18n.html`) where it have inserted the `i18n`-tags in the proper places, without touching the original file, and optionally also the `.json` translation file, so it [doesn't have to be done with a broswer's developer console](https://wiki.roll20.net/Character_Sheet_i18n#Step_Two.2C_Generating_the_Translation_File).

If everything worked out as it should, it should display a short `<filename>-i18n.html created!` on the command line. A simple testfile can be found [here](https://raw.githubusercontent.com/Roll20/roll20-character-sheets/master/D6StarWars/D6StarWars.html).

## Constraints

At it's current state, ACSI can only create `i18n`-tags for [standard text](https://wiki.roll20.net/Character_Sheet_i18n#Standard_Text), that doesn't contain many line-breaks or special characters. If a sheet contains lots of commented HTML code, ACSI might be thrown off and produce an unusable result. ACSI is also assumed to be used on a sheet containing no i18n-tags or HTML error, as it isn't yet capable of noticing existing tags nor does it check the validity of HTML syntax.

ACSI is written in C, and relies on a few Unix commands, so it needs to be run in a Linux enviroment to work(Project author uses Ubuntu 16.4 TLS/Linux Mint 19.1). Doesn't rely on any advanced libraries.

## Optional
If you run `./acsi <filename> t`, ACSI will  additionally print in the console the generated tagnames and their contentents.

If you run `./acsi <filename> j`, ACSI will generate the **translation file** with the name `<originalname>.json`.

If you run `./acsi <filename> a`, it will do both options mentioned above.

# Changelog

## V0.50 (2019-05-14)
- doesn't break html tags as often while creating the new file, some reason was due to poor transcribing by `savetxt()` function in some unusual linebreaks and unexpected characters
- can optionally generate the **translation file** alongside creating the translated version of the sheet
- doesn't any longer attempt to ignore html comments, sheetworkers and rolltemplates as the implementation did more harm than good. The advice is to strip the html file from rolltemplates and sheetworkers before being used with ACSI
- minor code cleanup

## V0.33 (2019-04-18)
- removed malfunctioning functions that were searching for `rolltemplate` and `script` sections. It's better to remove those sections from the code before running ACSI than working with the somewhat unreliable **V0.32**

## V0.32 (2019-03-06)
- fixed segfault
- uploaded v0.32 .deb package

## V0.31 (2019-02-20)
- add .gitattribute to force LF line endings

## V0.3 (2019-02-19)
- optional testmode added
- include a testfile

## V0.2 (2019-02-18)
- code/file cleanup
- fixed credits typo
- rewrote readme
- html comments, sheetworkers and rolltemplates doesn't  any longer have a negatively impact  when creating `i18n`-tags

## V0.1 (2018-11-30)
- make data tags for text displayed in html elements, `<p>like this text</p>`
- treats all text; comments, roll templates & sheetworkers alike, best to avoid if your sheet contains much of it
- autogenerated tags are at times clumsy, and duplicates may arise as it's currently taken in consideration

# Project Aim and Development Roadmap
The primary focus of the project is to prioritize on implementing the largest timesavers when it comes to creating `i18n`-tags, and in the end to automate as much as possible of the process. As a secondary goal it serves to improve programming/coding/documentation habits of the participants.

ASCI has a "two birds, one stone" origin from a 'C/Unix'-programming course, which explains the not too optimal choice of C for a text-parsing program.
If the interest arise, ACSI could be adapted/rewritten with other libraries/modules/languages for ease of further development, or just portability but it's of lower priority.

## 'Alpha' (V.0.7)
The aim for the Alpha version is to have ACSI reliably create 70% of the i18n-tags for any given sheet, and not to get thrown off by to many stray html comments or special characters in unexpected places.

- Ignore `cs>` , `cs<` , `cf>`, `cf<` found in some rolls (credits to David for noticing)
- Avoid creating duplicate tags and giving same tagname to sections that aren't identical to each other
- Skip `<script>`, `<rolltemplate>` and `<charmancer>` sections(sheetworkers) without altering them

## 'Main Goal'(V.1.0)
V.1.0 aims to reliably create 90% of i18n-tags for any given sheet, and provide more automation options, such as generating the associated `translation.json` file.

- make tags for [Element Attribute Text](https://wiki.roll20.net/Character_Sheet_i18n#Element_Attribute_Text) like `title`, `label` and `placeholder`
- make tags for [dropdown menu options](https://wiki.roll20.net/Character_Sheet_i18n#Dynamic_Key_Replacement)
- make tags for [Roll Templates](https://wiki.roll20.net/Character_Sheet_i18n#Roll_Templates)
- ~create the `translation.json` by itself (replicating the browser's [console.log()](https://wiki.roll20.net/Character_Sheet_i18n#Step_Two.2C_Generating_the_Translation_File) method)~ Implemented in V0.5
- push changes made to a `translation.json` file back to the sheet(so it will be easier to change the autogenerated tags)
- make tags for the more complex [Variable Replacement](https://wiki.roll20.net/Character_Sheet_i18n#Variable_Replacement) and [Roll Queries](https://wiki.roll20.net/Character_Sheet_i18n#Roll_Queries) methods


## 'Nice To Have'(V.1.5)
Optional things that could be done which goes beyond the primary goal if ACSI but could be useful or be a good learning experience to implement
- notice if files already have tags, and would only add missing tags
- rewriting to not rely on linux-processes(getting rid of `unistd.h` and `sys/.h` libraries), creating Windows/Mac version
- making tags for Charactermancer sections
- improve user-friendliness, a simple GUI?
- rework to use more standard command-line syntax
- refactoring search function
- Roll20 specific errorchecking that general html validators don't catch. Checking all `<input>`,`<textarea>` fields are named with `attr_` prefixes, warn not to use `id`-attributes and other known [Roll20-specific constrains](https://wiki.roll20.net/Building_Character_Sheets#Restrictions_.26_Security_Filtering)

# Glossary
- **RCSi18n**: [Roll20 Character Sheet i18n](https://wiki.roll20.net/Character_Sheet_i18n) standard
- **sheet**: [Character Sheet](https://wiki.roll20.net/Character_Sheets), consisting of a html and css file, as well as a translation file if it have been internationalized
- **tag, i18n-tag**: the unique tagname for a given text segment shown on a character sheet. It's the left side of the `translation.json`. Represented on the html sheet with the `data-i18n` [global attribute](https://www.w3schools.com/tags/ref_standardattributes.asp)
- **translation file**: the paired `translation.json` [needed]((https://wiki.roll20.net/Character_Sheet_i18n#Step_Two.2C_Generating_the_Translation_File)) to show a character sheet in Roll20
- **sheetworkers**: JavaScript included in the html sheet inside `<script>` tags [(Roll20Wiki page)](https://wiki.roll20.net/Sheet_Worker_Scripts)
- **roll templates**: html sections meant to change how dice rolls are display when rolled from buttons on the sheet [(Roll20Wiki page)](https://wiki.roll20.net/Roll_Templates)
