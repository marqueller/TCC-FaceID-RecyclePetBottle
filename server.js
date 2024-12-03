import express from 'express'
import publicRoutes from './routes/puclic.js'
import privateRoutes from './routes/private.js'

import auth from './middlewares/auth.js'

const app = express()
app.use(express.json()) // aqui eu informo que o app vai usar json

app.use('/', publicRoutes) // aqui eu estou usando o publicRoutes que foi importado
app.use('/', auth, privateRoutes) // aqui eu estou usando o privateRoutes que foi importado

app.listen(3000, () => console.log("Servidor Rodando!!"))

/* 3Rotas

Públicas
    Cadastro e login

Privadas
    Listar Usuarios

*/

/*Dados da conta MongoDB:
 - Usuário: marquellermdb1
 - Password: 988107635 
 - link: mongodb+srv://marquellermdb1:988107635@users.51mh1.mongodb.net/?retryWrites=true&w=majority&appName=Users
 */