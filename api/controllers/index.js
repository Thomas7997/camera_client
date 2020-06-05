const fs = require('fs');
const axios = require('axios');

const host = "192.168.0.91"
axios.defaults.baseURL = `http://${host}:8000`;

const txtToJson = txt => {
    var array = txt.split("\n");

    return array;
}

exports.take_capture = (req, res, next) => {
    fs.writeFile('../local/data/image/capture.txt', "1", (err) => {
        if (err) {
            console.log(err);
            return res.status(500).json({ msg : "Capture échouée." });
        }

        else {
            console.log("Ordre de capture envoyée.");
            res.json({ msg : "Ordre de capture envoyée." });
        }
    });
}

exports.get_images_list = (req, res, next) => {
    axios.get('/liste.txt')
    .then(response => {
        res.json(txtToJson(response.data));
    })
    .catch(err => res.status(500).json({ msg : "Erreur interne." }));
}