/*
    Copyright (c) 2008-2018 Jan W. Krieger (<jan@jkrieger.de>)

    

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License (LGPL) as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License (LGPL) for more details.

    You should have received a copy of the GNU Lesser General Public License (LGPL)
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



/**
 * \defgroup jkqtplotter_elements Plot Elements
 * \ingroup jkqtplotter
 * \defgroup jkqtplotter_plots Graphs
 * \ingroup jkqtplotter_elements
 */

/** \file jkqtpgraphs.h
  * \ingroup jkqtplotter
  */

#include <QString>
#include <QPainter>
#include <QPair>
#include "jkqtplotter/jkqtpbaseelements.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include "jkqtplottertools/jkqtptools.h"
#include "jkqtplottertools/jkqtp_imexport.h"
#include "jkqtplottertools/jkqtpimagetools.h"

#ifndef jkqtpgraphsbase_H
#define jkqtpgraphsbase_H

// forward declarations
class JKQtPlotter;
class JKQTPdatastore;

/** \brief this virtual base class of every element, which is part of a JKQtPlotter plot and may appear in its key
 *         (basically any type of graph, except overlay elements!)
 * \ingroup jkqtplotter_plots
 *
 * Each possible graph is represented by a child of this class. So additional plots may be created by
 * deriving new JKQTPgraph classes. To do so implement/overwrite these functions:
 *   - void draw(JKQTPEnhancedPainter& painter);
 *   - void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect);
 *   - bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero);
 *   - void getGraphsYMinMax(double& miny, double& maxy);
 *   - QColor getKeyLabelColor()=0;
 *   - bool usesColumn(int column) const;
 * .
 *
 * Optionally you may also overwrite these functions to draw elements outside the actual plot area (like e.g. colorbars):
 *   - void getOutsideSize(JKQTPEnhancedPainter& painter, int& leftSpace, int& rightSpace, int& topSpace, int& bottomSpace);
 *   - void drawOutside(JKQTPEnhancedPainter& painter, QRect leftSpace, QRect rightSpace, QRect topSpace, QRect bottomSpace);
 * .
 *
 */
class LIB_EXPORT JKQTPplotElement: public QObject {
        Q_OBJECT
    public:
        /** \brief class constructor */
        explicit JKQTPplotElement(JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        explicit JKQTPplotElement(JKQtPlotter* parent);

        /** \brief default wirtual destructor */
        virtual ~JKQTPplotElement() ;

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter)=0;

        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect)=0;

        /** \brief returns an image with a key marker inside  */
        QImage generateKeyMarker(QSize size=QSize(16,16));

        /** \brief get the maximum and minimum x-value of the graph
         *
         * The result is given in the two parameters which are call-by-reference parameters!
         */
        virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero)=0;
        /** \brief get the maximum and minimum y-value of the graph
         *
         * The result is given in the two parameters which are call-by-reference parameters!
         */
        virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero)=0;
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor()=0;

        JKQTPGET_SET_VMACRO(QString, title)
        JKQTPGET_SET_MACRO(bool, visible)

        /** \brief returns the parent painter class */
        inline JKQtBasePlotter* getParent() { return parent; }
        /** \brief sets the parent painter class */
        virtual void setParent(JKQtBasePlotter* parent);
        /** \brief sets the parent painter class */
        virtual void setParent(JKQtPlotter* parent);

        /*! \brief if the graph plots outside the actual plot field of view (e.g. color bars, scale bars, ...)

            \note If you want to draw outside, then you'll also have to implement drawOutside()
         */
        virtual void getOutsideSize(JKQTPEnhancedPainter& painter, int& leftSpace, int& rightSpace, int& topSpace, int& bottomSpace);

        /*! \brief plots outside the actual plot field of view (e.g. color bars, scale bars, ...)

            \note If you want to draw outside, then you'll also have to implement getOutsideSize(), so enough space is reserved

            The four value supplied tell the method where to draw (inside one of the rectangles).
         */
        virtual void drawOutside(JKQTPEnhancedPainter& painter, QRect leftSpace, QRect rightSpace, QRect topSpace, QRect bottomSpace);

    protected:
        /** \brief the plotter object this object belongs to */
        JKQtBasePlotter* parent;

        /** \brief title of the plot (for display in key!). If no title is supplied, no key entry is drawn. */
        QString title;

        /** \brief indicates whether the graph is visible in the plot */
        bool visible;



        /** \brief tool routine that transforms an x-coordinate for this plot element */
        virtual double transformX(double x) const;

        /** \brief tool routine that transforms a y-coordinate for this plot element */
        virtual double transformY(double y) const;

        /** \brief tool routine that backtransforms an x-coordinate for this plot element */
        virtual double backtransformX(double x) const;

        /** \brief tool routine that backtransforms a y-coordinate for this plot element */
        virtual double backtransformY(double y) const;


        /** \brief tool routine that transforms a QPointF according to the parent's transformation rules */
        inline QPointF transform(const QPointF& x) {
            return QPointF(transformX(x.x()), transformY(x.y()));
        }

        /** \brief tool routine that back-transforms a QPointF according to the parent's transformation rules */
        inline QPointF backTransform(const QPointF& x) {
            return QPointF(backtransformX(x.x()), backtransformY(x.y()));
        }

        /** \brief tool routine that transforms a QPointF according to the parent's transformation rules */
        inline QPointF transform(double x, double y) {
            return transform(QPointF(x,y));
        }
        /** \brief tool routine that back-transforms a QPointF according to the parent's transformation rules */
        inline QPointF backTransform(double x, double y) {
            return backTransform(QPointF(x,y));
        }
        /** \brief tool routine that transforms a QVector<QPointF> according to the parent's transformation rules */
        QVector<QPointF> transform(const QVector<QPointF>& x);

        /** \brief tool routine that transforms a QVector<QPointF> according to the parent's transformation rules
         *         and returns a (non-closed) path consisting of lines */
        QPainterPath transformToLinePath(const QVector<QPointF>& x);

        /** \brief tool routine that transforms a QVector<QPointF> according to the parent's transformation rules
         *         and returns a polygon */
        inline QPolygonF transformToPolygon(const QVector<QPointF>& x) {
            return QPolygonF(transform(x));
        }
};

/** \brief this virtual base class of the (data-column based) graphs,
 *         which are part of a JKQtPlotter plot and which use the coordinate system
 *         of the JKQtPlotter (i.e. the two coordinate axes get_xAxis() and get_yAxis())
 *         as basis for the graphs
 * \ingroup jkqtplotter_plots
 *
 * This class adds several features to work with data columns. In addition this class adds protected
 * functions that do coordinate transforms based on the current coordinate system, of the paren
 * JKQtPlotter (i.e. using the axes JKQtPlott:xAxis and JKQtPlotter::yAxis as basis for the plotting).
 *
 * There are two properties datarange_start and datarange_end. By default they are -1 and therefore ignored.
 * if they are != -1 the plotter only displays the datapoints with the indexes [datarange_start .. datarange_end]
 * although there might be more data points available (range [0 .. maxDataPoints]). The datarange is cut at the
 * full range, i.e. if datarange_end>maxDataPoints the plotter displays [datarange_start .. maxDataPoints].
 */
class LIB_EXPORT JKQTPgraph: public JKQTPplotElement {
        Q_OBJECT
    public:
        /** \brief class constructor */
        explicit JKQTPgraph(JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        explicit JKQTPgraph(JKQtPlotter* parent);

        /** \brief default wirtual destructor */
        virtual ~JKQTPgraph() ;


        JKQTPGET_SET_MACRO(int, datarange_start)
        JKQTPGET_SET_MACRO(int, datarange_end)

        /** \brief returns \c true if the given column is used by the graph */
        virtual bool usesColumn(int column) const;

    protected:
        /** \brief this function is used to plot error inidcators before plotting the graphs.
         *
         * By default this function does nothing. But children of this class may overwrite it to implement
         * drawing error indicators.
         */
        virtual void drawErrorsBefore(JKQTPEnhancedPainter& /*painter*/);
        /** \brief this function is used to plot error inidcators after plotting the graphs.
         *
         * By default this function does nothing. But children of this class may overwrite it to implement
         * drawing error indicators.
         */
        virtual void drawErrorsAfter(JKQTPEnhancedPainter& /*painter*/);





        /** \brief start of the range of plot data. -1 switches the lower data range border off. */
        int datarange_start;
        /** \brief end of the range of plot data. -1 switches the upper data range border off. */
        int datarange_end;

        /** \brief get the maximum and minimum value of the given column
         *
         * The result is given in the two parameters which are call-by-reference parameters!
         */
        bool getDataMinMax(int column, double& minx, double& maxx, double& smallestGreaterZero);

        friend class JKQTPgraphErrors;

};


/** \brief this is the virtual base class of all JKQTPplotElement's in a JKQtPlotter plot that
 *         represent geometric forms or annotations. They have extended coordinate transform capabilities, because
 *         in addition to using the plot coordinates, you can also choose to use different other
 *         coordinate systems
 * \ingroup jkqtplotter_plots
 *
 */
class LIB_EXPORT JKQTPplotObject: public JKQTPplotElement {
        Q_OBJECT
    public:
        /** \brief class constructor */
        explicit JKQTPplotObject(JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        explicit JKQTPplotObject(JKQtPlotter* parent);

        /** \brief default wirtual destructor */
        virtual ~JKQTPplotObject() ;


    protected:


};



/*! \brief This virtual JKQTPgraph descendent may be used as base for all graphs that use at least two columns
           that specify x and y coordinates for the single plot points.
    \ingroup jkqtplotter_plots

    This class implements basic management facilities for the data columns and implements the function
      - bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero);
      - bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero);
    .

 */
class LIB_EXPORT JKQTPxyGraph: public JKQTPgraph {
        Q_OBJECT
    public:
        enum DataSortOrder {
            Unsorted=0,
            SortedX=1,
            SortedY=2
        };


        /** \brief class constructor */
        JKQTPxyGraph(JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPxyGraph(JKQtPlotter* parent);

        /** \brief get the maximum and minimum x-value of the graph
         *
         * The result is given in the two parameters which are call-by-reference parameters!
         */
        virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) override;
        /** \brief get the maximum and minimum y-value of the graph
         *
         * The result is given in the two parameters which are call-by-reference parameters!
         */
        virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) override;

        /** \copydoc JKQTPgraph::usesColumn() */
        virtual bool usesColumn(int column) const override;

        JKQTPGET_SET_MACRO(int, xColumn)
        JKQTPSET_CAST_MACRO(size_t, int, xColumn)
        JKQTPGET_SET_MACRO(int, yColumn)
        JKQTPSET_CAST_MACRO(size_t, int, yColumn)
        JKQTPGET_SET_MACRO(DataSortOrder, sortData)
        /** \brief sets the property sortData to the specified \a __value. \details Description of the parameter sortData is: <CENTER>\copybrief sortData.</CENTER> \see sortData for more information */
        void set_sortData(int __value);

    protected:

        /** \brief the column that contains the x-component of the datapoints */
        int xColumn;
        /** \brief the column that contains the y-component of the datapoints */
        int yColumn;

        /** \brief if \c !=Unsorted, the data is sorted before plotting */
        DataSortOrder sortData;
        /** \brief this array contains the order of indices, in which to access the data in the data columns */
        QVector<int> sortedIndices;
        virtual void intSortData();
        /** \brief returns the index of the i-th datapoint (where i is an index into the SORTED datapoints)
         *
         * This function can beu used to get the correct datapoint after sorting the datapoints,
         * As sorting is done by sorting an index and not reordering the data in the columns themselves.
         * */
        inline  int getDataIndex(int i) {
            if (sortData==Unsorted) return i;
            return sortedIndices.value(i,i);
        }
};


/*! \brief This virtual JKQTPgraph descendent may be used as base for all graphs that use at least one column
           of data
    \ingroup jkqtplotter_plots


 */
class LIB_EXPORT JKQTPsingleColumnGraph: public JKQTPgraph {
        Q_OBJECT
    public:
        enum DataSortOrder {
            Unsorted=0,
            Sorted=1
        };
        /** \brief class constructor */
        JKQTPsingleColumnGraph(JKQtBasePlotter* parent=nullptr);
        JKQTPsingleColumnGraph(int dataColumn, JKQtBasePlotter* parent=nullptr);
        JKQTPsingleColumnGraph(int dataColumn, QColor color, Qt::PenStyle style=Qt::SolidLine, double lineWidth=2.0, JKQtBasePlotter* parent=nullptr);
        JKQTPsingleColumnGraph(JKQtPlotter* parent);
        JKQTPsingleColumnGraph(int dataColumn, JKQtPlotter* parent);
        JKQTPsingleColumnGraph(int dataColumn, QColor color, Qt::PenStyle style, double lineWidth, JKQtPlotter* parent);
        JKQTPsingleColumnGraph(int dataColumn, QColor color, Qt::PenStyle style, JKQtPlotter* parent);
        JKQTPsingleColumnGraph(int dataColumn, QColor color, JKQtPlotter* parent);
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor();

        JKQTPGET_SET_MACRO(int, dataColumn)
        JKQTPSET_CAST_MACRO(size_t, int, dataColumn)
        JKQTPGET_SET_MACRO(QColor, color)
        JKQTPGET_SET_MACRO(Qt::PenStyle, style)
        JKQTPGET_SET_MACRO(double, lineWidth)
        JKQTPGET_SET_MACRO(DataSortOrder, sortData)
        /** \brief sets the property sortData to the specified \a __value. \details Description of the parameter sortData is: <CENTER>\copybrief sortData.</CENTER> \see sortData for more information */
        void set_sortData(int __value);

        /** \copydoc JKQTPgraph::usesColumn() */
        virtual bool usesColumn(int c) const override;

    protected:
        /** \brief the column that contains the datapoints */
        int dataColumn;

        /** \brief which plot style to use from the parent plotter (via JKQtPlotterBase::getPlotStyle() and JKQtPlotterBase::getNextStyle() ) */
        int parentPlotStyle;

        /** \brief color of the graph */
        QColor color;
        /** \brief linestyle of the graph lines */
        Qt::PenStyle style;
        /** \brief width (pixels) of the graph */
        double lineWidth;

        QPen getLinePen(JKQTPEnhancedPainter &painter) const;

        /** \brief if \c !=Unsorted, the data is sorted before plotting */
        DataSortOrder sortData;
        /** \brief this array contains the order of indices, in which to access the data in the data columns */
        QVector<int> sortedIndices;
        virtual void intSortData();
        /** \brief returns the index of the i-th datapoint (where i is an index into the SORTED datapoints)
         *
         * This function can beu used to get the correct datapoint after sorting the datapoints,
         * As sorting is done by sorting an index and not reordering the data in the columns themselves.
         * */
        inline  int getDataIndex(int i) {
            if (sortData==Unsorted) return i;
            return sortedIndices.value(i,i);
        }


};






/*! \brief Descendents of this class add data fields for error indicators and methods to plot them to a class.
    \ingroup jkqtplotter_plots

    This class is meant to be used with multiple inheritance. I.e. if you implemented some kind of plot
    you may derive an error plot class in which you will have to overwrite the JKQTPgraph::drawErrorsBefor()
    or drawErrorsAfter() so it calls the plotErrorIndicators() method from this class. In addition this class
    will add some public datamemebers and methods to your class that allow to specify the properties of the
    error indicators (plot properties: color, width, ... and columns for the data).

 */
class LIB_EXPORT JKQTPgraphErrors {
    public:
        /** \brief class contructor */
        JKQTPgraphErrors(QColor graphColor=QColor("black"));
        virtual ~JKQTPgraphErrors();

        JKQTPGET_SET_MACRO(QColor, errorColor)
        JKQTPGET_SET_MACRO(Qt::PenStyle, errorStyle)
        JKQTPGET_SET_MACRO(double, errorWidth)
        JKQTPGET_SET_MACRO(QColor, errorFillColor)
        JKQTPGET_SET_MACRO(Qt::BrushStyle, errorFillStyle)
        JKQTPGET_SET_MACRO(double, errorbarSize)
        /** \brief returns true, if the error plots use the given column */
        virtual bool errorUsesColumn(int c) const;

        void setErrorColorFromGraphColor(QColor graphColor);

    protected:
        /** \brief color of the error lines/bars */
        QColor errorColor;
        /** \brief linestyle of the error lines/bars */
        Qt::PenStyle errorStyle;
        /** \brief width (pixels) of the error lines/bars */
        double errorWidth;
        /** \brief fill color for error polygons */
        QColor errorFillColor;
        /** \brief fill style for error polygons */
        Qt::BrushStyle errorFillStyle;
        /** \brief size of the error bar end markers [pixels] */
        double errorbarSize;

        /** \brief draw error indicators with the parameters defined in this class. The position of the datapoints is
         *         given by the \a xColumn and \a yColumn. It is also possible to specify a datarange. This method is called by
         *         the JKQTPgraph descendents */
        virtual void plotErrorIndicators(JKQTPEnhancedPainter& painter, JKQtBasePlotter* parent, JKQTPgraph* parentGraph,  int xColumn, int yColumn, int datarange_start=-1, int datarange_end=-1, double xrelshift=0, double yrelshift=0.0, const  QVector<int>* dataorder=nullptr)=0;

        /** \brief draw error indicators with the parameters defined in this class. The position of the datapoints is
         *         given by the \a xColumn and \a yColumn. It is also possible to specify a datarange. */
        void intPlotXYErrorIndicators(JKQTPEnhancedPainter& painter, JKQtBasePlotter* parent, JKQTPgraph* parentGraph, int xColumn, int yColumn, int xErrorColumn, int yErrorColumn, JKQTPerrorPlotstyle xErrorStyle, JKQTPerrorPlotstyle yErrorStyle, int datarange_start=-1, int datarange_end=-1, int xErrorColumnLower=-1, int yErrorColumnLower=-1, bool xErrorSymmetric=true, bool yErrorSymmetric=true, double xrelshift=0, double yrelshift=0.0, const QVector<int> *dataorder=nullptr);

        /** \brief this function can be used to set the color of the error indicators automatically
         *
         * return \c true and the colors to use, if applicable, the default implementation returns false */
        virtual bool intPlotXYErrorIndicatorsGetColor(JKQTPEnhancedPainter& painter, JKQtBasePlotter* parent, JKQTPgraph* parentGraph, int xColumn, int yColumn, int xErrorColumn, int yErrorColumn, JKQTPerrorPlotstyle xErrorStyle, JKQTPerrorPlotstyle yErrorStyle, int index, QColor& errorColor, QColor& errorFillColor);


        virtual double getXErrorU(int i, JKQTPdatastore* ds) const;
        virtual double getXErrorL(int i, JKQTPdatastore* ds) const;
        virtual double getYErrorU(int i, JKQTPdatastore* ds) const;
        virtual double getYErrorL(int i, JKQTPdatastore* ds) const;

        //** \brief plot a single error indicator */
        //void intPlotXYErrorIndicator(JKQTPEnhancedPainter& painter, double x, double xperror, double xmerror, double y, double yperror, double ymerror, JKQTPerrorPlotstyle xErrorStyle=JKQTPnoError, JKQTPerrorPlotstyle yErrorStyle=JKQTPnoError, double lastx=0, double lastxperror=0, double lastxmerror=0, double lasty=0, double lastyperror=0, double lastymerror=0);
};


/*! \brief This class adds data fields for error indicators in x direction to a JKQTPgraph descendent.
    \ingroup jkqtplotter_plots
    \see JKQTPgraphErrors
 */
class LIB_EXPORT JKQTPxGraphErrors: public JKQTPgraphErrors {
    public:
        /** \brief class contructor */
        JKQTPxGraphErrors(QColor graphColor=QColor("black"));

        JKQTPGET_SET_MACRO(bool, xErrorSymmetric)
        JKQTPGET_MACRO(int, xErrorColumnLower)
        JKQTPGET_MACRO(int, xErrorColumn)
        JKQTPGET_SET_MACRO(JKQTPerrorPlotstyle, xErrorStyle)
        /** \brief set the column from which to read the error values for x-error indicators */
        void set_xErrorColumn(int __value);
        /** \brief set the column from which to read the error values for lower x-error indicators (if not set [=-1], the xErrorColumn-values will be used for upper and lower error indicators) */
        void set_xErrorColumnLower(int __value);

        /** \brief returns true, if the error plots use the given column */
        virtual bool errorUsesColumn(int c) const override;

    protected:
        /** \brief the column that contains the error of the x-component of the datapoints */
        int xErrorColumn;
        /** \brief the column that contains the error of the x-component of the datapoints. This is used as the lower error length, if xErrorSymmetric \c ==false. */
        int xErrorColumnLower;
        /** \brief indicates whether the x-errors are symmetric (from one column only) */
        bool  xErrorSymmetric;
        /** \brief how to draw the errors (if available) of the x-value */
        JKQTPerrorPlotstyle xErrorStyle;

        /** \brief draw error indicators with the parameters defined in this class. The position of the datapoints is
         *         given by the \a xColumn and \a yColumn. It is also possible to specify a datarange. */
        virtual void plotErrorIndicators(JKQTPEnhancedPainter& painter, JKQtBasePlotter* parent, JKQTPgraph* parentGraph, int xColumn, int yColumn, int datarange_start=-1, int datarange_end=-1, double xrelshift=0, double yrelshift=0.0, const QVector<int> *dataorder=nullptr) override;

        virtual double getXErrorU(int i, JKQTPdatastore* ds) const override;
        virtual double getXErrorL(int i, JKQTPdatastore* ds) const override;

};


/*! \brief This class adds data fields for error indicators in y direction to a class.
    \ingroup jkqtplotter_plots
    \see JKQTPgraphErrors

 */
class LIB_EXPORT JKQTPyGraphErrors: public JKQTPgraphErrors {
    public:
        /** \brief class contructor */
        JKQTPyGraphErrors(QColor graphColor=QColor("black"));

        JKQTPGET_SET_MACRO(bool, yErrorSymmetric)
        JKQTPGET_MACRO(int, yErrorColumnLower)
        JKQTPGET_MACRO(int, yErrorColumn)
        JKQTPGET_SET_MACRO(JKQTPerrorPlotstyle, yErrorStyle)
        /** \copydoc JKQTPgraphErrors::errorUsesColumn() */
        virtual bool errorUsesColumn(int c) const override;

        /** \brief set the column from which to read the error values for y-error indicators */
        void set_yErrorColumn(int __value);
        /** \brief set the column from which to read the error values for lower y-error indicators (if not set [=-1], the xErrorColumn-values will be used for upper and lower error indicators) */
        void set_yErrorColumnLower(int __value);
    protected:
        /** \brief the column that contains the error of the x-component of the datapoints */
        int yErrorColumn;
        /** \brief how to draw the errors (if available) of the x-value */
        JKQTPerrorPlotstyle yErrorStyle;
        /** \brief indicates whether the y-errors are symmetric (from one column only) */
        bool  yErrorSymmetric;
        /** \brief the column that contains the error of the y-component of the datapoints. This is used as the lower error length, if yErrorSymmetric \c ==false. */
        int yErrorColumnLower;

        /** \brief draw error indicators with the parameters defined in this class. The position of the datapoints is
         *         given by the \a xColumn and \a yColumn. It is also possible to specify a datarange. */
        virtual void plotErrorIndicators(JKQTPEnhancedPainter& painter, JKQtBasePlotter* parent, JKQTPgraph* parentGraph, int xColumn, int yColumn, int datarange_start=-1, int datarange_end=-1, double xrelshift=0, double yrelshift=0.0, const QVector<int> *dataorder=nullptr) override;

        virtual double getYErrorU(int i, JKQTPdatastore* ds) const override;
        virtual double getYErrorL(int i, JKQTPdatastore* ds) const override;
};


/*! \brief This class adds data fields for error indicators in x and y direction to a class.
    \ingroup jkqtplotter_plots
    \see JKQTPgraphErrors

 */
class LIB_EXPORT JKQTPxyGraphErrors: public JKQTPgraphErrors {
    public:
        /** \brief class contructor */
        JKQTPxyGraphErrors(QColor graphColor=QColor("black"));
        JKQTPGET_SET_MACRO(bool, xErrorSymmetric)
        JKQTPGET_SET_MACRO(bool, yErrorSymmetric)
        JKQTPGET_MACRO(int, xErrorColumnLower)
        JKQTPGET_MACRO(int, xErrorColumn)
        JKQTPGET_MACRO(int, yErrorColumnLower)
        JKQTPGET_MACRO(int, yErrorColumn)
        JKQTPGET_SET_MACRO(JKQTPerrorPlotstyle, yErrorStyle)
        JKQTPGET_SET_MACRO(JKQTPerrorPlotstyle, xErrorStyle)
        /** \copydoc JKQTPgraphErrors::errorUsesColumn() */
        virtual bool errorUsesColumn(int c) const override;

        /** \brief set the column from which to read the error values for x-error indicators */
        void set_xErrorColumn(int __value);
        /** \brief set the column from which to read the error values for lower x-error indicators (if not set [=-1], the xErrorColumn-values will be used for upper and lower error indicators) */
        void set_xErrorColumnLower(int __value);
        /** \brief set the column from which to read the error values for x-error indicators */
        void set_yErrorColumn(int __value);
        /** \brief set the column from which to read the error values for lower x-error indicators (if not set [=-1], the xErrorColumn-values will be used for upper and lower error indicators) */
        void set_yErrorColumnLower(int __value);
        /** \brief set the column from which to read the error values for y-error indicators */
        void set_xErrorColumn(size_t __value);
        /** \brief set the column from which to read the error values for lower y-error indicators (if not set [=-1], the xErrorColumn-values will be used for upper and lower error indicators) */
        void set_xErrorColumnLower(size_t __value);
        /** \brief set the column from which to read the error values for y-error indicators */
        void set_yErrorColumn(size_t __value);
        /** \brief set the column from which to read the error values for lower y-error indicators (if not set [=-1], the xErrorColumn-values will be used for upper and lower error indicators) */
        void set_yErrorColumnLower(size_t __value);

    protected:
        /** \brief the column that contains the error of the x-component of the datapoints */
        int yErrorColumn;
        /** \brief how to draw the errors (if available) of the x-value */
        JKQTPerrorPlotstyle yErrorStyle;
        /** \brief the column that contains the error of the x-component of the datapoints */
        int xErrorColumn;
        /** \brief how to draw the errors (if available) of the x-value */
        JKQTPerrorPlotstyle xErrorStyle;
        /** \brief indicates whether the x-errors are symmetric (from one column only) */
        bool  xErrorSymmetric;
        /** \brief indicates whether the y-errors are symmetric (from one column only) */
        bool  yErrorSymmetric;
        /** \brief the column that contains the error of the x-component of the datapoints. This is used as the lower error length, if xErrorSymmetric \c ==false. */
        int xErrorColumnLower;
        /** \brief the column that contains the error of the y-component of the datapoints. This is used as the lower error length, if yErrorSymmetric \c ==false. */
        int yErrorColumnLower;

        /** \brief draw error indicators with the parameters defined in this class. The position of the datapoints is
         *         given by the \a xColumn and \a yColumn. It is also possible to specify a datarange. */
        virtual void plotErrorIndicators(JKQTPEnhancedPainter& painter, JKQtBasePlotter* parent, JKQTPgraph* parentGraph, int xColumn, int yColumn, int datarange_start=-1, int datarange_end=-1, double xrelshift=0, double yrelshift=0.0, const QVector<int> *dataorder=nullptr) override;

        virtual double getXErrorU(int i, JKQTPdatastore* ds) const override;
        virtual double getXErrorL(int i, JKQTPdatastore* ds) const override;
        virtual double getYErrorU(int i, JKQTPdatastore* ds) const override;
        virtual double getYErrorL(int i, JKQTPdatastore* ds) const override;
};







#endif // jkqtpgraphsbase_H
