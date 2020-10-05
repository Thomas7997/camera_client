const exif = require('exiftool');

fs.readFile('./img.txt', function (err, data) {
    if (err)
      throw err;
    else {
      exif.metadata(data, function (err, metadata) {
        if (err)
          throw err;
        else
          console.log(metadata);
      });
    }
  });