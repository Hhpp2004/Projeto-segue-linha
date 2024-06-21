const express = require("express");
const bodyParser = require("body-parser");
const sqlite3 = require("sqlite3").verbose();
const PORT = 3001;

const app = express();
app.use(bodyParser.json());

// Função para criar a tabela se ela ainda não existir
function createTable() {
    const db = new sqlite3.Database('./teste.db', (err) => {
        if (err) {
            console.error(err.message);
        }
        console.log('Connected to the SQlite database.');
    });

    const sqlCreateTable = `
        CREATE TABLE IF NOT EXISTS viagem (
            id INTEGER PRIMARY KEY,
            sensor_esd TEXT,
            sensor_meio TEXT,
            sensor_dir TEXT
        );
    `;

    db.run(sqlCreateTable, (err) => {
        if (err) {
            console.error(err.message);
        }
        console.log('Table created successfully.');
    });

    db.close();
}

// Chama a função para criar a tabela
createTable();

app.post("/salvar-percurso", (req, res) => {
    const { id, sensor_esd, sensor_meio, sensor_dir } = req.body;
    const db = new sqlite3.Database('./teste.db');

    const sql = `INSERT INTO viagem(id,sensor_esd,sensor_meio,sensor_dir) VALUES (?,?,?,?)`;

    db.run(sql, [id, sensor_esd, sensor_meio, sensor_dir], function (err) {
        if (err) {
            console.log(err.message);
            res.status(500).send("Erro ao salvar o arquivo");
        } else {
            console.log("Dados sendo salvo");
        }
        db.close();
        res.send("Arquivo salvo com sucesso!");
    });
});

app.get("/salvar", (req, res) => {
    res.status(200).send("ok");
});

// Abre o banco de dados apenas uma vez
let db = new sqlite3.Database('./teste.db', sqlite3.OPEN_READWRITE | sqlite3.OPEN_CREATE, (err) => {
    if (err) {
      console.error(err.message);
    } else {
      console.log('Conectado ao banco de dados.');
    }
});

// Exemplo de consulta para verificar se a conexão está funcionando
db.serialize(() => {
    db.each(`SELECT * FROM viagem`, (err, row) => {
        if (err) {
            console.error(err.message);
        } else {
            console.log(row);
        }
    });
});



app.listen(PORT, () => {
    console.log(`Porta de utilização é ${PORT}`);
    console.log(`http://localhost:${PORT}`);
});
