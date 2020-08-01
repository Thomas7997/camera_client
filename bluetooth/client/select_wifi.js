const fs = require('fs');

const get_wifi_list = data => {
    let wifi = [];
    let lines = [];
    let words = [];
    let item = 0;
    let values = [];
    let wordsIndex = 0;
    let str = "";
    let secondType = "";
    let connected = null;

    str = data.toString('utf-8');
    lines = str.split('\n');
    lines = lines.splice(1, lines.length);
    lines.forEach(line => {
        connected = line.charAt(0) === '*';

        secondType = "";
        wordsIndex = 0;
        words = line.split(' ');
        words.forEach(word => {
            if (word !== '') {
                values[wordsIndex++] = word;
            }
        });

        wifi[item++] = {
            ssid : values[0+connected],
            signal : parseInt(values[5+connected] / 25) + 1,
            keyType : values[7+connected] !== '--' ? ('Locked') : (''),
            connected
        }
    });

    return wifi;
}

fs.readFile('./wifi_list_log.txt', (err, data) => {
    if (err) throw err;
    
    networks = get_wifi_list(data); // Résultat final (liste des wifis à afficher

    console.log(networks);
});