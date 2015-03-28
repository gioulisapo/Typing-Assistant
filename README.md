# Typing-Assistant
<strong>Description</strong><br>
<p>A typing autocomplete programm (that suggest the 4 most popular words starting from the given prefix) for linux systems writen in C</p>
<p>It loads an external dictionary (words.txt provided) that it's default path is in $HOME/.dict/dictionary.txt to a trie struct.</p>
<p>Each letter is saved in a leafless node. Each node is connected to it's parent and it's child. The final letter of every word is save to a normal node that will however have a leaf (all non-word terminating character nodes are leafleass). So for example "bill" and "billy" will be saved in the same leafless nodes until "bill" the "y" then will be saved in a new node that will however have a leaf that points to the last node "l". In that leaf the frequency of the word "billy" is saved.
<p>After succesfully loading the dictionary and initializing the frequencies to 0. The user can start typing a word and press tab for up to 4 suggestions, he can then choose one of them. If the word is not available the user can continue typing and press spacbar. The word then will be saved in the file for future use</p>
<p>Every time a word is autocompleted the previous words of the line reapear on the screen if enter is stroked then a new line begins</p>
<strong>Usage</strong><br>
<p>./typing-assistant</p>
<p>Optional Arguments</p><p>-b [absolute path of dictionary]</p>