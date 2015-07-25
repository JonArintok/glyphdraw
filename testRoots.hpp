

void printTextBlock() {
  int i = 0;
  for (uint32_t row = 0; row < (uint32_t)UItextBlock.size.y; row++) {
    for (uint32_t col = 0; col < (uint32_t)UItextBlock.size.x; col++, i++) {
      uchar c = (uchar)UItextBlock.text[i];
      cout << c;
    }
    cout << endl;
  }
}

void buildsomeroots() {
  roots.push_back(Root("As your body floats down Third Street"));
  roots.push_back(Root("With the burn-smell factory closing up"));
  roots.push_back(Root("Yes it's sad to say you will romanticize"));
  roots.push_back(Root("All the things you've known before"));
  roots.push_back(Root("It was not not not so great"));
  roots.push_back(Root("It was not not not so great"));
  roots.push_back(Root("And as you take a bath in that beaten path"));
  roots.push_back(Root("There's a pounding at the door "));
  roots.push_back(Root(""));
  roots.push_back(Root("Well it's a mighty zombie talking of some love and posterity"));
  roots.push_back(Root("He says The good old days never say good-bye"));
  roots.push_back(Root("If you keep this in your mind:"));
  roots.push_back(Root("You need some lo-lo-loving arms"));
  roots.push_back(Root("You need some lo-lo-loving arms"));
  roots.push_back(Root("And as you fall from grace the only words you say are:"));
  roots.push_back(Root(""));
  roots.push_back(Root("Put your hand inside the puppet head"));
  roots.push_back(Root("Put your hand inside the puppet head"));
  roots.push_back(Root("Put your hand inside"));
  roots.push_back(Root("Put your hand inside"));
  roots.push_back(Root("Put your hand inside the puppet head"));
  roots.push_back(Root(""));
  roots.push_back(Root("Ads up in the subway are the work of someone"));
  roots.push_back(Root("Trying to please their boss"));
  roots.push_back(Root("And though the guy's a pig we all know what he wants"));
  roots.push_back(Root("Is just to please somebody else"));
  roots.push_back(Root("If the pu-pu-puppet head"));
  roots.push_back(Root("Was only bu-bu-busted in"));
  roots.push_back(Root("It'd be a better thing for everyone involved"));
  roots.push_back(Root("And we wouldn't have to cry"));
  roots.push_back(Root(""));
  roots.push_back(Root("Put your hand inside the puppet head"));
  roots.push_back(Root("Put your hand inside the puppet head"));
  roots.push_back(Root("Put your hand inside"));
  roots.push_back(Root("Put your hand inside"));
  roots.push_back(Root("Put your hand inside the puppet head"));
  roots.push_back(Root(""));
  roots.push_back(Root("Memo to myself: Do the dumb things I gotta do"));
  roots.push_back(Root("Touch the puppet head"));
  roots.push_back(Root(""));
  roots.push_back(Root("Quit my job down at the carwash"));
  roots.push_back(Root("Didn't have to write no one a goodbye note"));
  roots.push_back(Root("That said, The check's in the mail, and"));
  roots.push_back(Root("I'll see you in church, and don't you ever change"));
  roots.push_back(Root("If the pu-pu-puppet head"));
  roots.push_back(Root("Was only bu-bu-busted in"));
  roots.push_back(Root("I'll see you after school"));
  roots.push_back(Root(""));
  roots.push_back(Root("Put your hand inside the puppet head"));
  roots.push_back(Root("Put your hand inside the puppet head"));
  roots.push_back(Root("Put your hand inside"));
  roots.push_back(Root("Put your hand inside"));
  roots.push_back(Root("Put your hand inside the puppet head"));
  roots.push_back(Root(""));
  roots.push_back(Root("Put your hand inside the puppet head"));
  roots.push_back(Root("Put your hand inside the puppet head"));
  roots.push_back(Root("Put your hand inside"));
  roots.push_back(Root("Put your hand inside"));
  roots.push_back(Root("Put your hand inside the puppet head"));
  
  
  UItextBlock.size.x = 0;
  UItextBlock.size.y = roots.size();
  for (uint i = 0; i < roots.size(); i++) {
    if (roots[i].text.size() > (uint32_t)UItextBlock.size.x) {
      UItextBlock.size.x = roots[i].text.size();
    }
  }
  UItextBlock.text.resize(UItextBlock.size.pro());
  uint32_t i = 0;
  for (uint row = 0; row < roots.size(); row++) {
    uint col = 0;
    for (; col < roots[row].text.size(); col++, i++) {
      UItextBlock.text[i] = roots[row].text[col];
    }
    for (; col < (uint32_t)UItextBlock.size.x; col++, i++) {
      UItextBlock.text[i] = 0;
    }
  }
  //printTextBlock();
}
