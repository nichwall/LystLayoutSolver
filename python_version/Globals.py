HEIGHT = 3
#WIDTH  = 16
WIDTH  = 9
MAX_COUNTS = [0,4,5,8,4,2,2,5,1,5,3,5,2,1,1,0] # How many of each piece there can be. Using old version from Dec 27, 2015?

# Horizontally concat multiline strings
def horz_cat_string(str1, str2):
    splt_lines = zip(str1.split('\n'), str2.split('\n'))
    # Horizontal join
    res = '\n'.join([x+y for x,y in splt_lines])
    return res
