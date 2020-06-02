const fs = require('fs');

exports.take_capture = (req, res, next) => {
    fs.writeFile('../local/data/image/capture.txt', "1", (err) => {
        if (err) {
            console.log(err);
            return res.status(500).json({ msg : "Capture échouée." });
        }

        else {
            console.log("Ordre de capture envoyée");
            res.json({ msg : "Ordre de capture envoyée" });
        }
    });
}