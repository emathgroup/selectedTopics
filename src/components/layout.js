import React from 'react'
import Footer from './footer'
import Header from './header';
import layoutStyles from './modules/layout.module.scss'
const Layout = (props) =>{
    return(
        <div className={layoutStyles.container}>
            <div className={layoutStyles.content}>
            <Header />
            {props.children}
            </div>
            <Footer/>
        </div>
    )
}
export default Layout