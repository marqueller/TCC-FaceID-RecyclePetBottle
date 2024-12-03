import express from 'express'
import bcrypt from 'bcrypt' // criptografar a senha
import { PrismaClient } from '@prisma/client' //esse código veio do queryng the database (site do prisma)
import jwt from 'jsonwebtoken' //gerar token

const prisma = new PrismaClient() //esse código veio do queryng the database (site do prisma)
const router = express.Router()

const JWT_SECRET = process.env.JWT_SECRET

//Cadastro
router.post('/cadastro', async (req, res) => {
    try {
        const user = req.body

        const salt = await bcrypt.genSalt(10)
        const hashPassword = await bcrypt.hash(user.password, salt) // padrão de encriptação

        const userDB = await prisma.user.create({
            data: {
                email: user.email,
                name: user.name,
                password: hashPassword,
            },

        })

        res.status(201).json(userDB)
    } catch (error) {
        res.status(500).json({ message: "Erro no Servidor, tente novamente" })
    }
})

//LOGIN

router.post('/login', async (req, res) => {
    try {
        const userInfo = req.body

        //Busca o usuário no banco de dados
        const user = await prisma.user.findUnique({
            where: { email: userInfo.email },
        })

        //Verifica se o usuário existe dendto do banco
        if (!user) {
            return res.status(404).json({ message: "usuario inválido" })
        }
        //compara a senha do banco com a que o usuário digitou
        const isMatch = await bcrypt.compare(userInfo.password, user.password)
        if (!isMatch) {
            return res.status(400).json({ message: 'Senha inválida' })
        }

        //Gerar o Token JWT ***npm install jsonwebtoken***
        const token = jwt.sign({ id: user.id }, JWT_SECRET, {expiresIn: '1m'})

        res.status(200).json(token)
    } catch (err) {
        res.status(500).json({ message: "Erro no Servidor, tente novamente" })
    }
})

export default router
