/**
 * 登录页
 * @author ankoye
 */

// 切换登录注册表单
const loginTitle = document.getElementById('loginTitle')
const registerTitle = document.getElementById('registerTitle')
const form = document.querySelectorAll('.login-form-content')

loginTitle.onclick = loginF
function loginF() {
  loginTitle.classList.add('active')
  registerTitle.classList.remove('active')
  form[0].classList.remove('hidden')
  form[1].classList.add('hidden')
}
registerTitle.onclick = registerF
function registerF() {
  loginTitle.classList.remove('active')
  registerTitle.classList.add('active')
  form[0].classList.add('hidden')
  form[1].classList.remove('hidden')
}

// 登录
const loginForm = document.querySelectorAll('.login-input')
const loginHint = document.getElementsByClassName('login-hint')[0]
function login() {
  const user = {
    username: loginForm[0].value,
    password: loginForm[1].value
  }
  var result = findUser(user)
  console.log(result)
  if (result.code === 0) {
    loginHint.classList.add('hidden')
    saveCurrentUser(result.data)
    location.href = 'index.html'
  } else {
    loginHint.classList.remove('hidden')
  }
}

// 注册
const registerForm = document.querySelectorAll('.register-input')
const registerHint = document.getElementsByClassName('register-hint')[0]
function register() {
  // 验证用户名
  const r1 = /[a-zA-Z0-9]{3,15}$/
  if (!r1.test(registerForm[0].value)) {
    registerHint.innerHTML = '用户名在3-15位，包含字母和数字'
    registerHint.classList.remove('hidden')
    return
  } else {
    registerHint.classList.add('hidden')
  }
  // 验证密码
  const r2 = /\w{6,16}/
  if (!r2.test(registerForm[1].value)) {
    registerHint.innerHTML = '密码6-16位有效，不含特殊字符'
    registerHint.classList.remove('hidden')
    return
  } else {
    registerHint.classList.add('hidden')
  }
  // 验证确认密码
  if (!registerForm[2].value || registerForm[1].value !== registerForm[2].value) {
    registerHint.innerHTML = '两次密码不一致'
    registerHint.classList.remove('hidden')
    return 
  } else {
    registerHint.classList.add('hidden')
  }
  const user = {
    username: registerForm[0].value,
    password: registerForm[1].value
  }
  const result = saveUser(user)
  // 查询数据库
  if (result.code === 0) {
    registerHint.classList.add('hidden')
    for (const item of registerForm) {
      item.value = ''
    }
    alert('注册成功')
    loginF()
  } else {
    registerHint.innerHTML = result.msg
    registerHint.classList.remove('hidden')
  }
}