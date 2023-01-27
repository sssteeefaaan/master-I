const router = require("express").Router()
const {registerStudent, registerProfessor} = require("../utils/register")
const {Professor} = require("../models/professor")
const {Student} = require("../models/student")
const env = process.env

router.get('/', async(req, res)=>{
    res.redirect('/uns/0')
})

router.get('/:nav', async (req, res) => {
    let nav = req.params['nav']
    let items = []
    let professorListLink = "/uns/0"
    let studentListLink = "/uns/1"
    let facultyPortals = {
        'FTN': env["FTN_LINK"] || "http://localhost:9998/",
        'PMF': env["PMF_LINK"] || "http://localhost:9997/",
        'PRAVNI': env["PRAVNI_LINK"] || "http://localhost:9996/"
    }
    if(nav == '0'){
        items = await Professor.find()
    }else if(nav == '1'){
        items = await Student.find()
    }else
        return res.render('error')

    res.render('uns/index', { nav, items, facultyPortals, professorListLink, studentListLink })
})

router.post("/register-professor", async(req, res)=>{
    try{
        const result = { status:200, errors: Array() }
        const data = req.body
        if (!data){
            result.status = 400
            result.errors = Array({ server: ["Empty body!"] })
        }else{
            result.errors += await registerProfessor(data)
            if (result.errors)
                result.status = 400
        }
        console.log(result)
        res.status(result.status).send(result)
    }catch(e){
        console.log(e)
        res.status(500).send({
            errors: [e]
        })
    }
})

router.post("/register-student", async(req, res)=>{
    try{
        const result = { status:200, errors:[] }
        const data = req.body
        if (!data){
            result.status = 400
            result.errors = [{ server: ["Empty body!"] }]
        }else{
            result.errors += await registerStudent(data)
            if (result.errors)
                result.status = 400
        }
        console.log(result)
        res.status(result.status).send(result)
    }catch(e){
        console.log(e)
        res.status(500).send({
            errors: [e]
        })
    }
})

module.exports = router