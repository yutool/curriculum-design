// 获取当前登录用户，并设置导航栏
window.onload = function () {
  const result = getCurrentUser()
  const currentUser = document.getElementById('currentUser')
  const loginNav = document.getElementById('loginNav')
  const logoutNav = document.getElementById('logoutNav')
  if (result.data) {
    currentUser.innerHTML = result.data.username
    loginNav.classList.add('hidden')
    logoutNav.classList.remove('hidden')
  } else {
    currentUser.innerHTML = ''
    loginNav.classList.remove('hidden')
    logoutNav.classList.add('hidden')
  }
}

// 退出
function logout() {
  removeCurrentUser()
  location.href = 'login.html'
}

// 切换页面
const navs = document.querySelectorAll('.center-nav-item')
navs[0].onclick = () => {
  location.href = 'cart.html'
}
navs[1].onclick = () => {
  location.href = 'order.html'
}
navs[2].onclick = () => {
  location.href = 'recycle.html'
}

// 获取并渲染购物车列表
const userRecycle = document.getElementById('user-recycle')
var { data } = getRecycle()
console.log(data)
userRecycle.innerHTML = template('t-recycle', { recycles: data })

// 恢复订单
function handlerRecover(order) {
  addOrder(order)
  handlerDelete(order)
}

// 删除订单
function handlerDelete(order) {
  deleteRecycle(order.id)
  data = data.filter(item => item.id !== order.id)
  userRecycle.innerHTML = template('t-recycle', { recycles: data })
}
