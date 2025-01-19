PFont font;
JSONArray data;

void setup() {
  size(800, 600, P2D); // Use P2D renderer for the main canvas
  data = loadJSONArray("settings.json"); // Load the JSON file

  for (int i = 0; i < data.size(); i++) {
    JSONObject item = data.getJSONObject(i);
    
    // Read properties from JSON
    JSONArray textList = item.getJSONArray("text_list");
    String output = item.getString("output") + ".bmp";
    int textSize = item.getInt("size");

    // Set font with a large enough size to ensure clarity for scaling
    font = createFont("Cubic_11.ttf", 24); // Use a large font size for clarity
    textFont(font);

    // Calculate the actual dimensions of the output
    int textHeight = textSize * textList.size(); // Total height = size * number of lines
    int textWidth = 250; // Set fixed width or calculate dynamically if needed

    // Create a high-resolution graphics buffer for rendering
    PGraphics pg = createGraphics(textWidth * 10, textHeight * 10); // Scale up for clarity
    pg.beginDraw();
    pg.background(255); // White "transparent" background
    pg.textFont(font);
    pg.fill(0); // Black text
    pg.noStroke();
    pg.textSize(100); // Match the large font size used

    // Render each line of text
    for (int j = 0; j < textList.size(); j++) {
      pg.text(textList.getString(j), 0, 100 * (j + 1) - 10); // Adjust baseline alignment
    }
    pg.endDraw();

    // Downscale the image to the desired size using nearest neighbor
    PImage outputImage = pg.get();
    outputImage.resize(textWidth, textHeight); // Resize to exact pixel size
    outputImage.filter(THRESHOLD, 0.5); // Convert to strict black-and-white

    // Save the BMP file
    outputImage.save(output);
  }

  noLoop(); // Stop draw loop
}

void draw() {
  // Rendering is handled in setup()
}
