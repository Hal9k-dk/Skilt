void hal9klogo(){

  for (int i = 2; i < 70; i++){
    strip.setPixelColor(i, strip.Color(255,255,255)); // Moderately bright green color.

  }

  for (int i = 70; i < 116; i++){
    strip.setPixelColor(i, strip.Color(255,0,0)); // Moderately bright green color.

  }

  for (int i = 116; i < 200; i++){
    strip.setPixelColor(i, strip.Color(255,255,255)); // Moderately bright green color.

  }
    
    strip.show(); // Initialize all pixels to 'off'
  
  }
