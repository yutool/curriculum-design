/**
 * 购物车
 * @author ankoye
 */

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

// 获取并渲染购物车列表
const userCart = document.getElementById('user-cart')
const { data } = getCart()
userCart.innerHTML = template('t-cart', { carts: data })

// 选中的商品
var checkList = [] 

// 获取状态栏
const AllChecked = document.getElementById('AllChecked')
const countStatus = document.getElementById('count')
const moneyStatus = document.getElementById('money')

// 全选按钮
AllChecked.onclick = function() {
  const status = AllChecked.checked  // 获取当前状态
  if (status) {
    checkList = JSON.parse(JSON.stringify(data))
    for (const item of data) {
      document.getElementById(item.id).checked = true
    }
  } else {
    checkList = []
    for (const item of data) {
      document.getElementById(item.id).checked = false
    }
  }
  processState()
}

// 选中商品
function checkGoods(id) {
  var goods = null
  for (const item of data) if (item.id == id) {
    goods = item; break
  }
  const status = document.getElementById(id).checked  // 获取当前状态
  // 处理选中列表
  if (status) {
    checkList.push(goods)
  } else {
    checkList = checkList.filter(item => item.id != id)
  }
  processState()
}

// 状态栏处理
function processState() {
  // 处理单选框
  for (const item of checkList) {
    document.getElementById(item.id).checked = true
  }
  // 处理全选框
  if (checkList.length == data.length && checkList != 0) {
    AllChecked.checked = true
  } else {
    AllChecked.checked = false
  }

  // 处理状态栏
  var money = 0
  for (const item of checkList) {
    money += item.price * item.num
  }
  countStatus.innerHTML = checkList.length
  moneyStatus.innerHTML = money
}

// 数量减少按钮
function subBtn(id) {
  var num = 1
  for (const item of data) if (item.id == id) {
    num = item.num > 1 ? item.num - 1 : 1
    break
  }
  processNum(id, num) // 处理数量
}
// 数量增加按钮
function addBtn(id) {
  var num = 1
  for (const item of data) if (item.id == id) {
    num = item.num + 1
    break
  }
  processNum(id, num)
}
// 数量输入框
function numBtn(id) {
  var num = Number(document.getElementById('num_'+id).value)
  if (isNaN(num)) num = 1
  num = parseInt(num)
  if (num < 1) num = 1
  if (num > 999) num = 999
  processNum(id, num)
}

// 处理商品数量
function processNum(id, num) {
  // 设置数据库数量
  setCart(id, num)
  // 设置本地数量
  for (const item of data) if (item.id == id) {
    item.num = num
    break
  }
   // 设置选中的数量
  for (const item of checkList) if (item.id == id) {
    item.num = num
    break
  }
  // 显示数量
  document.getElementById('num_' + id).value = num
  // 更新状态栏
  processState()
}

// 删除商品
function processDelete(id) {
  // 删除数据库
  deleteCart(id)
  // 删除本地
  for (const i in data) if (data[i].id == id) {
    data.splice(i, 1); break;
  }
}

// 删除购物车商品
function deleteGoods(id) {
  console.log(id)
  processDelete(id)
  // 删除选中
  for (const i in checkList) if (checkList[i].id == id) {
    checkList.splice(i, 1); break;
  }
  // 重新渲染
  userCart.innerHTML = template('t-cart', { carts: data })
  // 更新状态栏
  processState()
}

// 删除选中
function batchDelete() {
  for (const item of checkList) {
    processDelete(item.id)
  }
  // 删除选中
  checkList = []
  // 重新渲染
  userCart.innerHTML = template('t-cart', { carts: data })
  // 更新状态栏
  processState()
}

// 付款
function pay() {
  if (checkList.length == 0) {
    alert('请选择商品')
    return
  }
  var value = prompt("请输入付款金额")
  while (value) {
    // 计算对比金额
    var money = 0
    for (const item of checkList) {
      money += item.price * item.num
    }
    if (value == money) {
      alert('付款成功')
      batchDelete()
      break
    } else {
      value = prompt("输入的金额有误，请重新输入")
    }
  }
}
