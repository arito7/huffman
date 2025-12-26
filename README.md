new keyword
if you don't use the new keyword when creating a struct it is made in stack memory, data created in stack memory dies when it reaches the closing curly bracket.
if you do use the new keyword it is allocated memory in the heap.
if you use the heap it will not be automatically garbage collected if you lose the pointer before deleting it you will create a memory leak.
