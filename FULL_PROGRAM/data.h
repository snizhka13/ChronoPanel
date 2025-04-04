static const char* messages[] = { 
  "There are only 10 types of people in the world: those who understand binary and those who don’t.",
  "Debugging is like being the detective in a crime movie where you are also the murderer.",
  "Software development: 10% coding, 90% figuring out why it doesn’t work.",
  "Why do programmers prefer dark mode? Because light attracts bugs!",
  "A programmer’s wife tells him: ‘Go to the store and get a loaf of bread. If they have eggs, get a dozen.’ He comes back with 12 loaves of bread.",
  "Real programmers count from 0.",
  "Computers do what you tell them to do, not what you want them to do.",
  "Programming is the art of debugging a blank screen."
};


struct Break {
  int startHour; 
  int startMinute; 
  int endHour; 
  int endMinute; 
};

static const Break breaks[] = {
  {10, 50, 11, 0},
  {12, 50, 13, 0}, 
  {14, 50, 15, 0}, 
  {16, 50, 17, 0}, 
  {18, 50, 19, 0} 
};

const int numBreaks = sizeof(breaks) / sizeof(breaks[0]);